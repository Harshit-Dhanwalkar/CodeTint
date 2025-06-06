// This is a C++ single-line comment
#include <iostream> // Preprocessor directive
#include <string>

/*
 * This is a
 * multi-line C++ comment.
 */

using namespace std; // Using directive

const int DEFAULT_VALUE = 42; // Global constant

// Function template
template <typename T>
T add(T a, T b) {
    return a + b;
}

class MyCppClass {
public: // Access specifier
    int publicVar;
    string name;

    // Constructor
    MyCppClass(string n) : name(n), publicVar(DEFAULT_VALUE) {
        cout << "MyCppClass object created: " << name << endl;
    }

    // Member function
    void displayMessage() const {
        cout << "Message from " << name << ": Hello C++!" << endl;
    }

    // Destructor
    ~MyCppClass() {
        cout << "MyCppClass object destroyed: " << name << endl;
    }
};

int main() {
    int num1 = 10;
    int num2 = 20;
    int sum = add(num1, num2); // Function call

    cout << "Sum using template: " << sum << endl;

    MyCppClass obj("Example"); // Object creation
    obj.displayMessage(); // Member function call

    bool isActive = true; // Boolean literal
    if (isActive) {
        cout << "Object is active." << endl;
    }

    return 0; // Return statement
}
