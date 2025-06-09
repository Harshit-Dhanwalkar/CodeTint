// test.rs
fn main() {
    // Print a message to the console
    println!("Hello, Rust from CodeTint!");

    // Declare a mutable variable
    let mut counter = 0;

    // Loop until counter reaches 5
    while counter < 5 {
        println!("Counter: {}", counter);
        counter += 1;
    }

    // Call a function
    greet("World");
}

// A simple function
fn greet(name: &str) {
    println!("Greetings, {}!", name);
}
