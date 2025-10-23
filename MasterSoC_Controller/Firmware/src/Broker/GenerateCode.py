from xml.etree.ElementPath import ops
import pandas as pd
import os
import re

def generate_header_guard(filename):
    # Convert filename to uppercase and replace non-alphanumeric characters with underscore
    guard = re.sub(r'\W+', '_', filename.upper())
    return guard

def prepare_typedef_groups(excel_file):
    """
    Reads the Excel file, processes the data, and returns typedef_groups.
    """
    # Read the Excel file
    df = pd.read_excel(excel_file)
    
    # Check that required columns exist
    required_columns = ["Return_t", "Group Name", "QT Label", 
                        "HEX OPCODE", "Callback", "P0_t", "P1_t", "P2_t", "P3_t", "P4_t", "P5_t"]
    for col in required_columns:
        if col not in df.columns:
            raise ValueError(f"Error: Excel file must contain columns: {required_columns}")

    # Filter out rows that contain "****" in any cell
    df = df[~df.apply(lambda row: row.astype(str).str.contains(r'\*{4}').any(), axis=1)]
    
    # Replace None/NaN in parameter columns with an empty string
    param_cols = ["P0_t", "P1_t", "P2_t", "P3_t", "P4_t", "P5_t"]
    df[param_cols] = df[param_cols].fillna("")

    # Deduplicate typedefs based on unique combinations of Return_t and parameter columns.
    grouped_df = (
        df.groupby(["Return_t"] + param_cols, as_index=False)
          .agg({
              "Group Name": lambda x: "_".join(sorted(set(str(name).strip().replace(" ", "_").lower() for name in x))),
              "QT Label": lambda x: "_".join(sorted(set(str(label).strip().replace(" ", "_").lower() for label in x))),
              "HEX OPCODE": lambda x: ", ".join(sorted(set(str(opcode).strip().upper() for opcode in x))),  # Concatenate all unique HEX OPCODEs
              "Callback": lambda x: ", ".join(sorted(set(str(callback).strip() for callback in x if pd.notna(callback)))),  # Concatenate all unique Callbacks
          })
    )

    # Add a new column to count how many parameter columns are non-empty
    grouped_df["num_params"] = grouped_df[param_cols].apply(lambda row: sum(1 for x in row if x != ""), axis=1)
    
    # Generate a typedef line using the Return_t, Group Name, and the parameter types.
    grouped_df["Typedef"] = grouped_df.apply(
        lambda row: f"typedef {row['Return_t']} {row['Group Name']}_callback(" +
                    ", ".join([str(row[col]) for col in param_cols if row[col] != ""]) +
                    ");",
        axis=1
    )

    # Group the resulting typedefs by the parameter count (ignoring those with 0 parameters)
    typedef_groups = grouped_df[grouped_df["num_params"] > 0].groupby("num_params")
    
    return typedef_groups

def generate_typedefs(excel_file, output_file):
    """
    Generates typedefs and writes them to the output file.
    """
    # Prepare typedef_groups
    typedef_groups = prepare_typedef_groups(excel_file)
    
    with open(output_file, 'w') as f:
        f.write("/* Auto-generated typedefs grouped by number of parameters */\n\n")
        f.write('#include "LMS7002M/LMS7002M.h"\n\n')
        # Process groups in order of increasing parameter count
        for num_params, group in sorted(typedef_groups, key=lambda x: x[0]):
            f.write(f"// Typedefs for functions with {num_params} parameter(s)\n")
            for idx, row in group.iterrows():
                f.write(row["Typedef"] + "\n")
            f.write("\n")
    

def sanitize_identifier(name):
    """Generate a valid identifier from a given name."""
    sanitized = re.sub(r'\W+', '_', name)
    return sanitized

def generate_opcode_descriptors(excel_file, output_file):
    # Read the Excel file
    df = pd.read_excel(excel_file)
    
    # Filter out rows that contain "****" in any cell
    df = df[~df.apply(lambda row: row.astype(str).str.contains(r'\*{4}').any(), axis=1)]
    
    # Required columns for opcode descriptor generation
    required_cols = [
        "Group Name",    # This will be used as the group identifier (enum constant)
        "HEX OPCODE",    # The opcode value (e.g. "A1" or "0xA1")
        "QT Label",      # API name (string)
        "P0_t", "P1_t", "P2_t", "P3_t", "P4_t", "P5_t",  # Parameter types
        "Callback"       # Callback function name
    ]
    for col in required_cols:
        if col not in df.columns:
            print("Error: Excel file must contain columns:", required_cols)
            return
    
    # Group the DataFrame by "Group Name"
    groups = df.groupby("Group Name")
    
    # Collect info for each group for the final aggregated array.
    group_info_list = []
    
    with open(output_file, 'w') as f:
        # Write the header includes and additional necessary headers
        f.write('#include "parser_typedefs.h"\n')
        f.write('#include "parser.h"\n')
        f.write('#include "LMS7002M_filter_cal.h"\n')
        #f.write('#include "Geric_Parameter.h" // Ensure Geric_Parameter is defined\n')
        #f.write('#include "opcode_constants.h" // Ensure opcode constants are defined\n\n')
        
        f.write("/* Auto-generated file: Grouped OpcodeDescriptor arrays and global all_descriptors array */\n\n")
        
        # For each unique group, generate an array of OpcodeDescriptor
        for group_name, group_df in groups:
            sanitized_group = sanitize_identifier(group_name)
            array_name = f"{sanitized_group}_opcodes"
            f.write(f"/* Opcode descriptors for group {group_name} */\n")
            f.write(f"OpcodeDescriptor {array_name}[] = {{\n")
            
            for index, row in group_df.iterrows():
                hex_opcode = str(row["HEX OPCODE"]).strip()
                # Ensure the opcode is a valid hex literal (prepend "0x" if needed)
                if not hex_opcode.lower().startswith("0x"):
                    hex_opcode = "0x" + hex_opcode
                qt_label = str(row["QT Label"]).strip()
                callback = (str(row["Callback"]).strip() 
                            if pd.notna(row["Callback"]) and str(row["Callback"]).strip() not in ["", "None"]
                            else "NULL")
                
                # Process parameter columns: only include those that are not None
                param_cols = ["P0_t", "P1_t", "P2_t", "P3_t", "P4_t", "P5_t"]
                param_types_list = []
                for col in param_cols:
                    value = row[col]
                    if pd.notna(value):
                        value_str = str(value).strip()
                        if value_str and value_str.lower() != "none":
                            param_types_list.append(value_str)
                num_params = len(param_types_list)
                comment_hint = ", ".join(param_types_list) if param_types_list else "none"
                
                f.write("    {\n")
                # Note: individual descriptors do not include a .group field.
                f.write("        .opcode = {0},\n".format(hex_opcode))
                f.write("        .QT_Label = \"{0}\",\n".format(qt_label))
                f.write("        .num_params = {0},\n".format(num_params))
                # Set .args to NULL with a comment showing the non-None parameter types.
                f.write("        .args = NULL, // " + comment_hint + "\n")
                f.write("        .callback = (void*){0}\n".format(callback))
                f.write("    },\n")
            
            f.write("};\n\n")
            # Save the group info (enum constant from "Group Name" and array name) for the global pointer array.
            group_info_list.append((group_name, array_name))
        
        # Generate the global array of pointers to OpcodeDescriptor arrays.
        f.write("// Create a global array of all OpcodeDescriptor arrays\n")
        f.write("OpcodeDescriptor* all_descriptors[] = {\n")
        for group_name, array_name in group_info_list:
            f.write("    {0},\n".format(array_name))
        f.write("};\n\n")

        # Generate the sizes of array of pointers to OpcodeDescriptor arrays.
        f.write("//Define sizes for each group\n")
        f.write("const size_t group_sizes[] = {\n")
        for group_name, array_name in group_info_list:
            f.write("    sizeof({0}) / sizeof(OpcodeDescriptor),\n".format(array_name))
        f.write("};\n\n")

        
        # Write the getOpcodeDescriptor function using a mask-based approach
        f.write("/*\n")
        f.write(" * @brief Searches for an OpcodeDescriptor by opcode using a mask for efficiency.\n")
        f.write(" *\n")
        f.write(" * This function extracts the group ID and sub-index (SN) from the opcode\n")
        f.write(" * using bitwise operations, allowing direct access to the descriptor.\n")
        f.write(" *\n")
        f.write(" * @param opcode The opcode to search for.\n")
        f.write(" * @return A pointer to the matching OpcodeDescriptor, or NULL if not found.\n")
        f.write(" */\n")
        f.write("OpcodeDescriptor* getOpcodeDescriptor(uint32_t opcode) {\n")
        f.write("    // Extract the group ID (first 5 bits of the opcode)\n")
        f.write("    uint8_t Group_ID = opcode & 31; // Mask to get the lower 5 bits\n")
        f.write("    // Extract the sub-index (remaining bits of the opcode)\n")
        f.write("    uint8_t SN = opcode >> 5; // Shift right to get the sub-index\n")
        f.write("\n")
        f.write("    // Validate the group ID to ensure it is within bounds\n")
        f.write("    if (Group_ID >= sizeof(all_descriptors) / sizeof(all_descriptors[0])) {\n")
        f.write("        return NULL; // Invalid group ID\n")
        f.write("    }\n")
        f.write("\n")
        f.write("    // Get the group of descriptors\n")
        f.write("    OpcodeDescriptor* group_desc = all_descriptors[Group_ID];\n")
        f.write("    // Get the size of the group\n")
        f.write("    size_t group_size = group_sizes[Group_ID];\n")
        f.write("\n")
        f.write("    // Validate the sub-index to ensure it is within bounds\n")
        f.write("    if (SN >= group_size) {\n")
        f.write("        return NULL; // Invalid sub-index\n")
        f.write("    }\n")
        f.write("\n")
        f.write("    // Return the matching descriptor\n")
        f.write("    return &group_desc[SN];\n")
        f.write("}\n")
    
    print("Opcode descriptors generated in file:", output_file)

def generate_execute_opcode(excel_file, output_file):
    """
    Generates the executeOpcode function based on the Excel file.
    """
    # Prepare typedef_groups using the existing function
    typedef_groups = prepare_typedef_groups(excel_file)

    # Open the output file for writing the auto-generated C code
    with open(output_file, 'w') as f:
        # Write the function header
        f.write("#include \"parser.h\" \n")
        f.write("#include \"parser_typedefs.h\" \n\n")
        f.write("/**\n")
        f.write(" * @brief Executes the callback function for a given opcode.\n")
        f.write(" *\n")
        f.write(" * This function retrieves the OpcodeDescriptor for the given opcode, validates\n")
        f.write(" * the number of parameters, and dynamically calls the callback function with\n")
        f.write(" * the arguments provided in the buffer.\n")
        f.write(" *\n")
        f.write(" * @param opcode The opcode to execute.\n")
        f.write(" * @param buffer An array of Geric_Parameter containing the arguments.\n")
        f.write(" * @param buffer_size The number of arguments in the buffer.\n")
        f.write(" * @return int Returns 0 on success, or an error code on failure.\n")
        f.write(" */\n")
        f.write("int executeOpcode(LMS7002M_t *lms, uint32_t opcode, Geric_Parameter* buffer, size_t buffer_size) {\n")
        f.write("    // Retrieve the descriptor for the given opcode\n")
        f.write("    OpcodeDescriptor* descriptor = getOpcodeDescriptor(opcode);\n")
        f.write("    if (descriptor == NULL) {\n")
        f.write("        fprintf(stderr, \"Error: Opcode 0x%X not found.\\n\", opcode);\n")
        f.write("        return -1; // Opcode not found\n")
        f.write("    }\n\n")
        f.write("    // Validate the number of parameters\n")
        f.write("    if (descriptor->num_params != buffer_size) {\n")
        f.write("        fprintf(stderr, \"Error: Invalid number of arguments for opcode 0x%X. Expected %d, got %zu.\\n\",\n")
        f.write("                opcode, descriptor->num_params, buffer_size);\n")
        f.write("        return -2; // Invalid number of arguments\n")
        f.write("    }\n\n")
        f.write("    // Ensure the callback function is defined\n")
        f.write("    if (descriptor->callback == NULL) {\n")
        f.write("        fprintf(stderr, \"Error: No callback defined for opcode 0x%X.\\n\", opcode);\n")
        f.write("        return -3; // No callback defined\n")
        f.write("    }\n\n")
        f.write("    // Dynamically call the callback function based on the number of parameters\n")
        f.write("    switch (descriptor->num_params) {\n")

        # Generate switch cases for each unique number of parameters
        for num_params, group in sorted(typedef_groups, key=lambda x: x[0]):
            f.write(f"        case {num_params}: {{\n")
            f.write("          switch (opcode) { \n")
            for idx, row in group.iterrows():
                typedef_name = row["Group Name"] + "_callback"
                opcode = [f"0x{x.strip()}" for x in row["HEX OPCODE"].split(",")]
                callbackNames = row["Callback"]
                formatted_callbacks = "/"+"*"*180+"\n\t\t\t* " + "\n\t\t\t* ".join(x.strip() for x in callbackNames.split(",")) + "\n\t\t\t"+"*"*180+"/"

               # str_opcode = ", ".join(opcode)
                f.write(f"            {formatted_callbacks}\n")
                for str_opcode in opcode:
                    f.write(f"            case {str_opcode}:\n")
                

                param_casts = []
                for i in range(num_params):
                    param_type = str(row[f"P{i}_t"]).strip()
                    if param_type == "int":
                        param_casts.append(f"buffer[{i}].value.sint")
                    elif param_type == "double":
                        param_casts.append(f"buffer[{i}].value.d")
                    elif param_type == "LMS7002M_t *":
                        param_casts.append(f"lms")
                    elif param_type == "bool":
                        param_casts.append(f"buffer[{i}].value.b")
                    elif param_type == "size_t":
                        param_casts.append(f"buffer[{i}].value.size")
                    elif param_type == "const char *":
                        param_casts.append(f"buffer[{i}].value.string")
                    elif param_type == "const int":
                        param_casts.append(f"buffer[{i}].value.const_int")
                    elif param_type == "const LMS7002M_chan_t":
                        param_casts.append(f"buffer[{i}].value.const_chan")
                    elif param_type == "const LMS7002M_dir_t":
                        param_casts.append(f"buffer[{i}].value.const_dir")
                    elif param_type == "const LMS7002M_port_t":
                        param_casts.append(f"buffer[{i}].value.const_port")
                    elif param_type == "const bool":
                        param_casts.append(f"buffer[{i}].value.const_bool")
                    elif param_type == "const double":
                        param_casts.append(f"buffer[{i}].value.const_double")
                    elif param_type == "double *":
                        param_casts.append(f"buffer[{i}].value.double_ptr")
                    else:
                        param_casts.append(f"NULL")  # Default to void*

                # Write the callback casting and invocation
                callback_signature = ", ".join(param_casts)
                f.write(f"              (({typedef_name}*)descriptor->callback)({callback_signature});\n")
                f.write(f"              break;\n\n")
            f.write("            } \n")
            f.write("            break;\n")
            f.write("        }\n")

        # Default case for unsupported number of parameters
        f.write("        default:\n")
        f.write("            fprintf(stderr, \"Error: Unsupported number of parameters (%d) for opcode 0x%X.\\n\",\n")
        f.write("                    descriptor->num_params, opcode);\n")
        f.write("            return -4; // Unsupported number of parameters\n")
        f.write("    }\n\n")
        f.write("    return 0; // Success\n")
        f.write("}\n")

    print("executeOpcode function generated in file:", output_file)


if __name__ == '__main__':
    # Build the file path relative to this script's location
    script_dir = os.path.dirname(os.path.abspath(__file__))
    excel_file = os.path.abspath(os.path.join(script_dir, "../../../../ApiDataBase/AutoGenCode.xlsx"))
    # Output header file name
    output_file = os.path.abspath(os.path.join(script_dir,"parser_typedefs.h"))
    generate_typedefs(excel_file, output_file)
    
    # Output C source file name (for example, "parser_opcodes.c")
    output_file = os.path.abspath(os.path.join(script_dir,"parser_opcodes.c"))
    generate_opcode_descriptors(excel_file, output_file)

    output_file = os.path.abspath(os.path.join(script_dir,"execute_opcode.c"))
    generate_execute_opcode(excel_file, output_file)
