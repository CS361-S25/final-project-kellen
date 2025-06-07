#ifndef TASK_H
#define TASK_H

#include <cmath>
#include <string>
#include <iostream>

/// Converts a float to a boolean.
/// Considers any non-zero value as true.
bool toBool(float x) {
    return std::fabs(x) > 0.001;
}

/// Converts a boolean to a float.
/// Returns 1.0f for true, 0.0f for false.
float toFloat(bool b) {
    return b ? 1.0f : 0.0f;
}

/// Abstract base class for all task types.
/// Defines the interface that all task types must implement to check outputs and return task names.
class Task {
public:
    /// Checks if the output is correct for a given set of inputs.
    /// @param output The output to check.
    /// @param inputs The input values for the task.
    /// @return A score indicating correctness (e.g., 1.0 for correct, 0.0 for incorrect).
    virtual double CheckOutput(float output, float inputs[4]) = 0;

    /// Returns the name of the task.
    /// @return The name of the task as a string.
    virtual std::string name() const = 0;
};

// NOT Gate: Inverts the input.
class NOT : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool input = toBool(inputs[0]);
        bool result = !input;
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "NOT";
    }
};

// NAND Gate: Outputs true unless both inputs are true.
class NAND : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = !(toBool(inputs[0]) && toBool(inputs[1]));
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "NAND";
    }
};

// AND Gate: Outputs true only if both inputs are true.
class AND : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = toBool(inputs[0]) && toBool(inputs[1]);
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "AND";
    }
};

// OR_N Gate: Multi-input OR (true if any input is true).
class OR_N : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = false;
        for (int i = 0; i < 4; ++i) {
            result = result || toBool(inputs[i]);
        }
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "OR_N";
    }
};

// OR Gate: Outputs true if at least one of the two inputs is true.
class OR : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = toBool(inputs[0]) || toBool(inputs[1]);
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "OR";
    }
};

// AND_N Gate: Multi-input AND (true only if all inputs are true).
class AND_N : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = true;
        for (int i = 0; i < 4; ++i) {
            result = result && toBool(inputs[i]);
        }
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "AND_N";
    }
};

// NOR Gate: Outputs true only if both inputs are false.
class NOR : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = !(toBool(inputs[0]) || toBool(inputs[1]));
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "NOR";
    }
};

// XOR Gate: Outputs true if inputs are different.
class XOR : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = toBool(inputs[0]) ^ toBool(inputs[1]);
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "XOR";
    }
};

// EQU Gate: Logical equivalence (XNOR), outputs true if inputs are the same.
class EQU : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool result = !(toBool(inputs[0]) ^ toBool(inputs[1]));
        return (std::fabs(output - toFloat(result)) < 0.001) ? 0.0 : 0.0;
    }

    std::string name() const override {
        return "EQU";
    }
};

// COMPLEX: (A AND B) OR (C AND D)
class COMPLEX : public Task {
public:
    double CheckOutput(float output, float inputs[4]) override {
        bool ab = toBool(inputs[0]) && toBool(inputs[1]);
        bool cd = toBool(inputs[2]) && toBool(inputs[3]);
        bool result = ab || cd;
        return (std::fabs(output - toFloat(result)) < 0.001) ? 64.0 : 0.0;
    }

    std::string name() const override {
        return "COMPLEX";
    }
};


#endif // TASK_H
