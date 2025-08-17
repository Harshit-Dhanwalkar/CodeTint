// ========== C++ Test File for Pager Functionality ==========
// This file contains a comprehensive set of C++ features to test syntax highlighting
// and pager functionality. It's intentionally long to require multiple pages.

#include <iostream>   // Standard I/O
#include <vector>     // Vector container
#include <string>     // String class
#include <memory>     // Smart pointers
#include <algorithm>  // STL algorithms
#include <cmath>      // Math functions
#include <fstream>    // File I/O
#include <exception>  // Exception handling
#include <thread>     // Multithreading
#include <mutex>      // Mutex for thread safety
#include <atomic>     // Atomic operations
#include <chrono>     // Time utilities
#include <map>        // Map container
#include <set>        // Set container
#include <functional> // Function objects

using namespace std;  // Using directive (for demo purposes)

// ========== Constants and Macros ==========
#define MAX_ITERATIONS 100
const double PI = 3.141592653589793;
const string APP_NAME = "C++ Pager Test";

// ========== Global Variables ==========
atomic<int> globalCounter(0);
mutex coutMutex;

// ========== Function Declarations ==========
template <typename T>
T calculateArea(T radius);

void processData(const vector<int>& data);
void threadWorker(int id);
void demonstrateSTL();
void handleExceptions();
void fileOperations();
void lambdaExamples();

// ========== Classes and Structs ==========

// Base class
class Shape {
protected:
    string name;
public:
    explicit Shape(const string& n) : name(n) {}
    virtual ~Shape() = default;
    virtual double area() const = 0;
    virtual void display() const {
        cout << "Shape: " << name << endl;
    }
};

// Derived class
class Circle : public Shape {
    double radius;
public:
    Circle(double r) : Shape("Circle"), radius(r) {}
    double area() const override {
        return PI * radius * radius;
    }
    void display() const override {
        Shape::display();
        cout << "Radius: " << radius << ", Area: " << area() << endl;
    }
};

// Template class
template <typename T>
class DataContainer {
    vector<T> data;
public:
    void add(const T& item) {
        data.push_back(item);
    }
    void process() {
        sort(data.begin(), data.end());
    }
    void print() const {
        for (const auto& item : data) {
            cout << item << " ";
        }
        cout << endl;
    }
};

// ========== Main Function ==========
int main() {
    cout << "=== Starting C++ Pager Test Program ===" << endl;
    cout << "Application: " << APP_NAME << endl;

    // Basic operations
    int x = 10;
    int y = 20;
    int sum = x + y;
    cout << "Sum: " << sum << endl;

    // Template function usage
    double circleArea = calculateArea(5.5);
    cout << "Area of circle with radius 5.5: " << circleArea << endl;

    // STL demonstrations
    demonstrateSTL();

    // Multithreading
    thread t1(threadWorker, 1);
    thread t2(threadWorker, 2);
    t1.join();
    t2.join();

    // Exception handling
    handleExceptions();

    // File operations
    fileOperations();

    // Lambda examples
    lambdaExamples();

    // Object oriented examples
    Circle c(7.5);
    c.display();

    DataContainer<int> intContainer;
    for (int i = 0; i < 10; ++i) {
        intContainer.add(rand() % 100);
    }
    intContainer.process();
    cout << "Sorted integers: ";
    intContainer.print();

    cout << "=== Program Completed ===" << endl;
    return 0;
}

// ========== Function Implementations ==========

template <typename T>
T calculateArea(T radius) {
    return PI * radius * radius;
}

void processData(const vector<int>& data) {
    int sum = 0;
    for (int num : data) {
        sum += num;
    }
    cout << "Data sum: " << sum << endl;
}

void threadWorker(int id) {
    for (int i = 0; i < 5; ++i) {
        {
            lock_guard<mutex> lock(coutMutex);
            cout << "Thread " << id << " working (" << i << ")" << endl;
        }
        this_thread::sleep_for(chrono::milliseconds(100));
        globalCounter++;
    }
}

void demonstrateSTL() {
    cout << "\n=== STL Demonstration ===" << endl;

    // Vector
    vector<int> numbers = {5, 2, 8, 1, 9};
    cout << "Original vector: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;

    // Algorithm
    sort(numbers.begin(), numbers.end());
    cout << "Sorted vector: ";
    for (int n : numbers) cout << n << " ";
    cout << endl;

    // Map
    map<string, int> wordCount = {
        {"hello", 3},
        {"world", 5},
        {"cpp", 2}
    };
    cout << "\nWord counts:\n";
    for (const auto& pair : wordCount) {
        cout << pair.first << ": " << pair.second << endl;
    }

    // Set
    set<string> uniqueWords = {"apple", "banana", "apple", "orange"};
    cout << "\nUnique words: ";
    for (const string& word : uniqueWords) {
        cout << word << " ";
    }
    cout << endl;
}

void handleExceptions() {
    cout << "\n=== Exception Handling ===" << endl;

    try {
        vector<int> vec(5);
        cout << "Attempting to access out of bounds..." << endl;
        cout << vec.at(10); // This will throw
    } catch (const out_of_range& e) {
        cerr << "Out of range error: " << e.what() << endl;
    } catch (const exception& e) {
        cerr << "Standard exception: " << e.what() << endl;
    } catch (...) {
        cerr << "Unknown exception occurred" << endl;
    }
}

void fileOperations() {
    cout << "\n=== File Operations ===" << endl;

    ofstream outFile("test.txt");
    if (outFile) {
        outFile << "This is a test file.\n";
        outFile << "Created by " << APP_NAME << "\n";
        outFile.close();
        cout << "File written successfully" << endl;
    }

    ifstream inFile("test.txt");
    if (inFile) {
        string line;
        cout << "File contents:\n";
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    } else {
        cerr << "Failed to open file for reading" << endl;
    }
}

void lambdaExamples() {
    cout << "\n=== Lambda Examples ===" << endl;

    auto square = [](int x) { return x * x; };
    cout << "Square of 5: " << square(5) << endl;

    vector<int> nums = {1, 2, 3, 4, 5};
    cout << "Original numbers: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // Transform with lambda
    transform(nums.begin(), nums.end(), nums.begin(),
        [](int x) { return x * 2; });
    
    cout << "Doubled numbers: ";
    for (int n : nums) cout << n << " ";
    cout << endl;

    // Capture example
    int threshold = 4;
    auto countAbove = count_if(nums.begin(), nums.end(),
        [threshold](int x) { return x > threshold; });
    cout << "Numbers above " << threshold << ": " << countAbove << endl;
}

// ========== More Code to Extend File Length ==========

namespace ExtraCode {
    class ComplexNumber {
        double real, imag;
    public:
        ComplexNumber(double r, double i) : real(r), imag(i) {}
        
        ComplexNumber operator+(const ComplexNumber& other) const {
            return ComplexNumber(real + other.real, imag + other.imag);
        }
        
        friend ostream& operator<<(ostream& os, const ComplexNumber& cn) {
            os << cn.real << " + " << cn.imag << "i";
            return os;
        }
    };

    void runExamples() {
        cout << "\n=== Extra Code Examples ===" << endl;
        
        ComplexNumber a(1.5, 2.5);
        ComplexNumber b(3.0, -1.5);
        ComplexNumber c = a + b;
        
        cout << "Complex numbers:\n";
        cout << "a = " << a << "\nb = " << b << "\nc = a + b = " << c << endl;
        
        // More template examples
        DataContainer<string> stringContainer;
        stringContainer.add("zebra");
        stringContainer.add("apple");
        stringContainer.add("banana");
        stringContainer.process();
        cout << "Sorted strings: ";
        stringContainer.print();
    }
}

// Even more code to ensure the file is long enough...

void additionalFunctions() {
    cout << "\n=== Additional Functions ===" << endl;
    
    // Demonstrate more C++ features
    auto smartPtr = make_unique<int>(42);
    cout << "Smart pointer value: " << *smartPtr << endl;
    
    // Type inference
    auto message = "Hello Modern C++";
    cout << message << endl;
    
    // Initializer lists
    for (int n : {1, 2, 3, 5, 8, 13}) {
        cout << n << " ";
    }
    cout << endl;
    
    // Structured bindings
    map<string, int> ageMap = {{"Alice", 30}, {"Bob", 25}};
    for (const auto& [name, age] : ageMap) {
        cout << name << " is " << age << " years old" << endl;
    }
}

// Final block of code to push the file length over multiple pages...

class FinalExamples {
public:
    static void run() {
        cout << "\n=== Final Examples ===" << endl;
        
        // Demonstrate more advanced features
        function<int(int)> fib = [&fib](int n) {
            return n <= 1 ? n : fib(n-1) + fib(n-2);
        };
        
        cout << "Fibonacci(10) = " << fib(10) << endl;
        
        // Time measurement
        auto start = chrono::high_resolution_clock::now();
        int result = fib(20);
        auto end = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start);
        
        cout << "Fibonacci(20) = " << result 
             << " (took " << duration.count() << " ms)" << endl;
    }
};

// ========== End of File ==========
