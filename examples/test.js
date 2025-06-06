// This is a single-line JavaScript comment

/*
 * This is a
 * multi-line JavaScript comment.
 */

const APP_NAME = "MyJSApp"; // Constant variable
let counter = 0; // Mutable variable

// Function declaration
function greet(name) {
  const greeting = `Hello, ${name}!`; // Template literal
  console.log(greeting); // console.log is a built-in function
  return greeting;
}

// Arrow function expression
const incrementCounter = () => {
  counter++;
  return counter;
};

// Class declaration
class Person {
  constructor(name, age) {
    this.name = name;
    this.age = age;
  }

  sayHello() {
    console.log(`Hi, my name is ${this.name} and I am ${this.age} years old.`);
  }

  static appInfo() {
    console.log(`Running on ${APP_NAME}`);
  }
}

// Control flow
if (counter < 5) {
  for (let i = 0; i < 3; i++) {
    incrementCounter();
  }
}

// Function calls
greet("World");
Person.appInfo(); // Static method call
const john = new Person("John Doe", 30); // Constructor call
john.sayHello(); // Method call

// Array and Object literals
const fruits = ["apple", "banana", "cherry"];
const config = {
  url: "http://example.com",
  timeout: 5000,
  isActive: true,
};

console.log(fruits[0]);
console.log(config.url);
