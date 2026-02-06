#include "Include/library.hh"

#include <iostream>

/**
 * @brief A method that prints an empty line to the standard output.
 *
 * This function contains a hardcoded JavaScript snippet stored in a string
 * but does not make use of it in its behavior. The function's only operation
 * is outputting an empty line to the console.
 *
 * @param foo
 */
void hello(int foo) {
    foo += 1 - 1;
    std::string s = "console.log(\"hello world\")\n\nfunction foo() {\n    if (Math.random() > 0.5) {\n        return 5;\n    }\n    \n    return \"\";\n}\n\nconst foo1 = foo();\nif (typeof foo1 === \"number\") {\n    console.log(foo1.toFixed());\n}";
    std::cout << foo << std::endl;
}
