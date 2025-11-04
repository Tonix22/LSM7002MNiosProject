#ifndef PARSER_H
#define PARSER_H

/**
 * @file parser.h
 * @brief Definitions and function prototypes for parsing and opcode handling in LMS7002M.
 *
 * This header file defines various macros, enumerations, data structures, and function
 * prototypes used for parameter parsing and command processing in the LMS7002M system.
 */

#include "LMS7002M/LMS7002M.h"
#include "util.h"

/**
 * @def MAX_PARAMETERS
 * @brief Maximum number of parameters allowed.
 */
#define MAX_PARAMETERS 5

/**
 * @brief Macro to access the signed integer value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define INT(n) Params[n].value.sint

/**
 * @brief Macro to access the string value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define STR(n) Params[n].value.string

/**
 * @brief Macro to access the enumerated type value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define ENUM(n) Params[n].value.enum_type

/**
 * @brief Macro to access the constant integer value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define CONSINT(n) Params[n].value.const_int

/**
 * @brief Macro to access the boolean value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define BOOLEAN(n) Params[n].value.b

/**
 * @brief Macro to access the double-precision value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define DOUBLE(n)  Params[n].value.d

/**
 * @brief Macro to access a pointer to a double-precision value.
 *
 * @param n Index of the parameter in the Params array.
 */
#define DOUBLE_POINTER(n)  Params[n].value.d_pointer

/**
 * @brief Macro to access a pointer to a short integer.
 *
 * @param n Index of the parameter in the Params array.
 */
#define SHORT_POINTER(n)  Params[n].value.short_p

/**
 * @brief Macro to access the size value of a parameter.
 *
 * @param n Index of the parameter in the Params array.
 */
#define SIZE_TYPE(n) Params[n].value.size

/**
 * @brief Searches for a specific ID within the LMS7002M device.
 *
 * This function examines the provided LMS7002M_t instance for a given identifier,
 * typically used to locate a specific register or configuration element.
 *
 * @param lms Pointer to an LMS7002M_t structure representing the device.
 * @param ID The identifier to search for.
 */
void search_by_ID(LMS7002M_t *lms, int ID);

/**
 * @enum OPCODE_enum
 * @brief Enumeration of opcodes corresponding to various operations.
 *
 * Each enumerator represents a distinct command or operation related to the LMS7002M
 * hardware configuration and control.
 */
typedef enum 
{
    CREATE_NUM,                     /**< Create a new configuration or instance. */
    ONE_PARAM_LMS7002M_T_NUM,       /**< Operation involving one LMS7002M parameter. */
    SPI_WRITE_NUM,                  /**< SPI write operation. */
    SPI_CONFIG_NUM,                 /**< SPI configuration operation. */
    INI_NUM,                        /**< Initialization operation. */
    CONFIGURE_LML_PORT_NUM,         /**< Configure the LML port. */
    ONE_PARAM_CONST_BOOL_NUM,       /**< Operation with a single constant boolean parameter. */
    ONE_PARAM_LMS7002M_CHAN_NUM,    /**< Operation involving one LMS7002M channel. */
    TWO_PARAM_LMS7002M_DIR_INT_NUM, /**< Operation involving LMS7002M direction and an integer. */
    LDO_ENABLE_NUM,                 /**< Enable/disable Low-Dropout (LDO) regulator. */
    AFE_ENABLE_NUM,                 /**< Enable/disable the Analog Front End (AFE). */
    SET_DATA_CLOCK_NUM,             /**< Set the data clock configuration. */
    SET_NCO_FREQ_NUM,               /**< Set the Numerically Controlled Oscillator (NCO) frequency. */
    SET_GFIR_TAPS_NUM,              /**< Set the number of taps for the GFIR filter. */
    SET_LO_FREQ_NUM,                /**< Set the Local Oscillator (LO) frequency. */
    TWO_PARAM_LMS_CONST_BOOL_NUM,   /**< Operation involving an LMS7002M parameter and a constant boolean. */
    TWO_PARAM_CHANT_SIZET_NUM,      /**< Operation involving a channel and a size parameter. */
    SP_TSG_NUM,                     /**< Special operation (possibly related to test signal generation). */
    TXSTP_CORRECTION_NUM,           /**< Correct the TX start/stop timing. */
    RXTSP_NUM,                      /**< Operation related to RX TSP (test signal processing). */
    SET_PATH_AND_BAND_NUM,          /**< Set the signal path and band configuration. */
    TBB_LOOP_BACK_ENABLE_NUM,       /**< Enable/disable loopback on the TBB (transceiver baseband). */
    BB_FILER_SET_NUM,               /**< Configure the baseband filter parameters. */
    TRF_RBB_RFE_NUM,                /**< Operation related to TRF, RBB, or RFE configurations. */
    READRSSI_NUM,                   /**< Read the Received Signal Strength Indicator (RSSI). */
    OPCODE_SIZE,                    /**< Total number of opcodes defined. */
} Group_name_enum;

/**
 * @struct Geric_Parameter
 * @brief A generic parameter type that can store various data types.
 *
 * This structure uses a union to allow storage of different data types
 * in a single variable, enabling flexible parameter handling.
 */
typedef struct Geric_Parameter {
    union {
        bool b;               /**< Boolean value. */
        char enum_type;       /**< Enumerated type value (stored as a char). */
        int sint;             /**< Signed integer value. */
        const int const_int;  /**< Constant integer value. */
        unsigned int uint;    /**< Unsigned integer value. */
        const char* string;         /**< Pointer to a null-terminated string. */
        short* short_p;       /**< Pointer to a short integer. */
        long l;               /**< Long integer value. */
        double d;             /**< Double precision floating point value. */
        double* d_pointer;    /**< Pointer to a double precision floating point value. */
        size_t size;          /**< Size value (commonly used for lengths or counts). */
        uint32_t bit_32;      /**< 32-bit unsigned integer, often for bit-level operations. */
       // const LMS7002M_chan_t const_chan;
        LMS7002M_chan_t const_chan;
      // const LMS7002M_dir_t const_dir;
        LMS7002M_dir_t const_dir;
        //const LMS7002M_port_t const_port;
        LMS7002M_port_t const_port;
        const bool const_bool;
        const double const_double;
        double* double_ptr;
    } value;                  /**< Union to hold the actual parameter value. */
} Geric_Parameter;

/*
 * Structure to hold opcode descriptor information.
 *
 * Each descriptor contains:
 *  - The opcode value (as generated by LimeGUI).
 *  - An internal opcode ID (from OPCODE_enum).
 *  - The API name and description.
 *  - The expected number of parameters and their types.
 *
 * NOTE: In practice, this table should be auto-generated from your Excel files.
 */
typedef struct {
    uint32_t opcode;              /* Opcode in hexadecimal */
    const char* QT_Label;         /* API name as produced by LimeGUI */
    int num_params;               /* Number of parameters expected */
    Geric_Parameter* args; /* Array of parameter types */
    void *callback;               /* Callback function pointer */
} OpcodeDescriptor;

OpcodeDescriptor* getOpcodeDescriptor(uint32_t opcode);

// Prototype for an opcode callback function (currently commented out).
// void opcode_callback(struct ad9361_rf_phy *phy);

/**
 * @brief Pushes data into a special memory area.
 *
 * This function writes the data pointed to by the provided memory pointer to a
 * designated special memory region.
 *
 * @param mem Pointer to the memory location containing the data to be pushed.
 */
void push_special (uint32_t* mem);

/**
 * @brief Pulls data from a special memory area.
 *
 * This function retrieves data from the special memory region and stores it in the
 * location pointed to by the provided memory pointer.
 *
 * @param mem Pointer to the memory location where the pulled data will be stored.
 */
void pull_special(uint32_t* mem);

/**
 * @brief Pushes a parameter into the parameter array.
 *
 * This function writes a parameter value into a parameter array at the specified index.
 *
 * @param var The parameter value to be pushed.
 * @param idx The index in the parameter array where the value will be stored.
 */
void push_param(uint32_t var, unsigned char idx);

/**
 * @brief Configures the opcode to parameter types mapping.
 *
 * This function sets up the parameter types based on the provided opcode, ensuring that
 * subsequent parsing and execution use the correct parameter interpretations.
 *
 * @param opcode The opcode corresponding to the operation to be configured.
 */
void set_opcode_to_ptypes(long opcode);

#endif /* PARSER_H */
