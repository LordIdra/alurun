#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <map>

using std::map;
using std::vector;
using std::string;

#define raise_error(line, error) std::cout << "[ERROR] line " << line << ": " << error;



inline void set_input_A(bool bit_8, bool bit_4, bool bit_2, bool bit_1) {
    // TODO toggle GPIO pins
}

inline void set_input_B(bool bit_8, bool bit_4, bool bit_2, bool bit_1) {
    // TODO toggle GPIO pins
}

inline void set_control_signals(bool INVERT, bool INCREMENT) {
    // TODO toggle GPIO pins
}

inline void set_multiplex_signals(bool signal_2, bool signal_1) {
    // TODO toggle GPIO pins
}

inline void read_output_signals(bool &signal_1, bool &signal_2, bool &signal_3, bool &signal_4) {
    // TODO read GPIO pins
}


inline int get_address(string instruction) {
    return stoi(instruction.substr(4, 3));
}





int main(int argc, char** argv) {

    // Location of the program we want to run
    string file_location = argv[0];

    // Open file and retrieve code, reading it into a vector
    std::ifstream file ("program.txt");

    file.seekg(0, std::ios::end);
    int file_size = file.tellg();
    file.seekg(0, std::ios::beg);
    
    vector<string> instructions;
    string line;

    while (std::getline(file, line)) instructions.push_back(line);

    file.close();


    // Allocate 10 thousand bytes (10 kilobytes) of RAM (we can access all of these w/ 4 digit codes)
    bool memory[10000][4];

    // Virtual accumulator and program counter
    bool accumulator[4] = {0, 0, 0, 0};
    unsigned int program_counter = 0;

    // Loop until end-of-file reached
    bool end_of_file = false;

    // Software-specific instruction enumeration
    map<string, int> software_instructions {
        {"INC", 0},
        {"DEC", 1},

        {"STO", 2},
        {"LDO", 3},

        {"BRA", 4},
        {"BRZ", 5},
        {"BRP", 6},

        {"INP", 7},
        {"OUT", 8},

        {"PRT", 9},
        {"HLT", 10}
    };


    while (program_counter < instructions.size()) {

        program_counter++;
        string instruction = instructions[program_counter];

        // Standard instructions
        if (instruction[0] == 'A') {

            bool *data = memory[stoi(instruction.substr(5, 4))];

            set_control_signals(instruction[1] - '0', instruction[2] - '0');
            set_multiplex_signals(instruction[3] - '0', instruction[4] - '0');

            set_input_A(accumulator[0], accumulator[1], accumulator[2], accumulator[3]);
            set_input_B(data[0], data[1], data[2], data[3]);
        } 
        
        // Software instruction
        else {

            switch (software_instructions[instructions[program_counter].substr(1, 3)]) {
                
                case 0:
                    // INC - Increment the accumulator
                    set_control_signals(0, 1);
                    set_multiplex_signals(0, 0);

                    set_input_A(accumulator[0], accumulator[1], accumulator[2], accumulator[3]);
                    set_input_B(0, 0, 0, 0);

                    read_output_signals(accumulator[0], accumulator[2], accumulator[3], accumulator[4]);


                case 1:
                    // DEC - Decrement the accumulator
                    set_control_signals(1, 0);
                    set_multiplex_signals(0, 0);

                    set_input_A(accumulator[0], accumulator[1], accumulator[2], accumulator[3]);
                    set_input_B(0, 0, 0, 0);
                    
                    read_output_signals(accumulator[0], accumulator[2], accumulator[3], accumulator[4]);


                case 2:
                    // STO - Stores the value in the accumulator at given address
                    int address = get_address(instructions[program_counter]);

                    memory[address][0] = accumulator[0];
                    memory[address][1] = accumulator[1];
                    memory[address][2] = accumulator[2];
                    memory[address][3] = accumulator[3];


                case 3:
                    // LDO - Load the value at the given address into the accumulator
                    int address = get_address(instructions[program_counter]);
                
                    accumulator[0] = memory[address][0];
                    accumulator[1] = memory[address][1];
                    accumulator[2] = memory[address][2];
                    accumulator[3] = memory[address][3];


                case 4:
                    // BRA - Set the program counter to provided value
                    int address = get_address(instructions[program_counter]);
                    program_counter = address;

                
                case 5:
                    // BRZ - Set the program counter to provided value if accumulator == 0
                    if ((accumulator[0] + accumulator[1] + accumulator[2] + accumulator[3]) == 0) {
                        int address = get_address(instructions[program_counter]);
                        program_counter = address;
                    }

                
                case 6:
                    // BRP - Set the program counter to provided value if accumulator >= 0
                    if ((accumulator[0] + accumulator[1] + accumulator[2] + accumulator[3]) >= 0) {
                        int address = get_address(instructions[program_counter]);
                        program_counter = address;
                    }

                
                case 7:
                    // INP - Load data at permanent storage address into accumulator
                    // TODO

                
                case 8:
                    // OUT - Set permanent storage address to accumulator
                    // TODO

                
                case 9:
                    // PRT - Prints the accumulator to the console
                    std::cout << std::to_string(accumulator[0]) << std::to_string(accumulator[1]) << std::to_string(accumulator[2]) << std::to_string(accumulator[3]) << "\n";

                
                case 10:
                    // HLT - Halt the program
                    break;
            }
        }
    }

    // Return 0 to indicate successful execution
    return 0;
}