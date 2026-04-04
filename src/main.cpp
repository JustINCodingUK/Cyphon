#include <iostream>
#include <fstream>

#include "analyzer/SemanticAnalyzer.h"
#include "emitter/PythonEmitter.h"
#include "lexer/Token.h"
#include "lexer/Lexer.h"
#include "parser/Parser.h"

int main(int argc, char** argv) {
    if (argc<2) {
        std::cout << "\033[1;31mERROR: \033[0m" << "No input files\nExiting now\n" << std::endl;;
        return 1;
    }
    if (argc==2 && std::string(argv[1])=="--info") {
        std::cout << "\n"
" /$$$$$$$$                  /$$                                             /$$$$$$       /$$\n"
"|__  $$__/                 | $$                                            /$$$_  $$    /$$$$\n"
   "   | $$ /$$   /$$  /$$$$$$ | $$$$$$$   /$$$$$$  /$$$$$$$        /$$    /$$| $$$$\\ $$   |_  $$\n"
   "   | $$| $$  | $$ /$$__  $$| $$__  $$ /$$__  $$| $$__  $$      |  $$  /$$/| $$ $$ $$     | $$\n"
   "   | $$| $$  | $$| $$  \\ $$| $$  \\ $$| $$  \\ $$| $$  \\ $$       \\  $$/$$/ | $$\\ $$$$     | $$\n"
   "   | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$| $$  | $$        \\  $$$/  | $$ \\ $$$     | $$\n"
   "   | $$|  $$$$$$$| $$$$$$$/| $$  | $$|  $$$$$$/| $$  | $$         \\  $/   |  $$$$$$//$$ /$$$$$$\n"
   "   |__/ \\____  $$| $$____/ |__/  |__/ \\______/ |__/  |__/          \\_/     \\______/|__/|______/\n"
        "        /$$  | $$| $$\n"
       "       |  $$$$$$/| $$\n"
        "       \\______/ |__/\n";
        std::cout << "\n\n";
        std::cout << "A Transpiler for a language we all love but can't use in large scale systems.\n";
        std::cout << "- Static type safety\n";
        std::cout << "- Extension functions without monkey patching\n";
        std::cout << "- Generics, IN PYTHON YES\n";
        std::cout << "- No ugly indentation, only braces\n";
        std::cout << "- Lambdas which can go beyond a single line\n";
        std::cout << "- A visibility system which is not a gentlemen's agreement\n";
        std::cout << "- And possible support for an LLVM backend";

        std::cout << "\n\nUsage: typhon <input_file>.ty\n";
        return 0;
    }
    for (int i=1;i<argc;++i) {
        if (std::ifstream file(argv[i]); file.is_open()) {
            std::string contents((std::istreambuf_iterator(file)), std::istreambuf_iterator<char>());

            Lexer lexer;
            auto tokens = lexer.tokenize(contents);
            std::cout << "Lexing complete\n";


            Parser parser(tokens);

            auto ast = parser.parse();
            std::cout << "Parsing complete\n";

            SemanticAnalyzer analyzer;
            ast = analyzer.analyze(std::move(ast));

            PythonEmitter pyEmitter(std::move(ast));
            pyEmitter.writeToFile(std::string(argv[i])+".py");
        } else {
            std::cerr << "Could not open file" << argv[i] << "\n";
        }
    }
}
