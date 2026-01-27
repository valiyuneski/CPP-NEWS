#include <iostream>
#include <concepts>
#include <type_traits>

// Concept defining the Shape interface
template<typename T>
concept Shape = requires(const T shape) {
    { shape.draw() } -> std::same_as<void>;
    { shape.area() } -> std::same_as<double>;
};

// Concrete implementation 1
class Circle {
private:
    double radius;
public:
    Circle(double r) : radius(r) {}
    
    void draw() const {
        std::cout << "Drawing a circle\n";
    }
    
    double area() const {
        return 3.14159 * radius * radius;
    }
};

// Concrete implementation 2
class Rectangle {
private:
    double width, height;
public:
    Rectangle(double w, double h) : width(w), height(h) {}
    
    void draw() const {
        std::cout << "Drawing a rectangle\n";
    }
    
    double area() const {
        return width * height;
    }
};

// Function that uses std::same_as to validate types
template<typename T>
void renderShape(const T& shape) {
    // Compile-time checks using std::same_as
    static_assert(std::same_as<T, Circle> || std::same_as<T, Rectangle>,
                  "Type must be Circle or Rectangle");
    
    shape.draw();
    std::cout << "Area: " << shape.area() << "\n\n";
}

// Alternative: Function using std::same_as in requires clause
template<typename T>
    requires (std::same_as<T, Circle> || std::same_as<T, Rectangle>)
void processShape(const T& shape) {
    std::cout << "Processing shape...\n";
    shape.draw();
}

int main() {
    Circle circle(5.0);
    Rectangle rect(4.0, 6.0);
    
    renderShape(circle);      // OK - T is Circle
    renderShape(rect);        // OK - T is Rectangle
    
    processShape(circle);     // OK
    processShape(rect);       // OK
    
    // renderShape(42);       // Compile error!
    
    // Runtime type checking example
    if constexpr (std::same_as<decltype(circle), Circle>) {
        std::cout << "circle is a Circle type\n";
    }
    
    return 0;
}