#include <iostream>

using namespace std;

struct Block
{
    int index;
    Block* next;

    int gate;
    int keycard;

    Block(){}

    Block(int index, int* gates, int* keycards)
    {
        this->index = index;
        this->gate = gates[index];
        this->keycard = keycards[index];
        this->next = nullptr;
    }
};

struct Monotonic_stack
{
    Block* top=nullptr;

    bool is_empty()
    {
        return top == nullptr;
    }

    void pop()
    {
        if (top == nullptr)
        {
            return;
        }

        Block* new_top = top->next;
        delete top;
        top = new_top;
    }

    int push(Block* block)
    {
        while (!is_empty() && block->gate >= top->gate)
        {
            pop();
        }

        if (top == nullptr)
        {
            top = block;
        }else
        {
            block->next = top;
            top = block;
        }

        int top_keycard = block->keycard;
        int top_index = block->index;

        while (!is_empty() && top_keycard >= top->gate)
        {
            pop();
        }

        if (is_empty())
        {
            return top_index + 1;
        }
        else
        {
            return top_index - top->index;
        }
    }

    ~Monotonic_stack()
    {
        while (!is_empty())
        {
            this->pop();
        }
    }
};

int* solve(int n, int gates[], int keycards[]) {
    // Solve the reachability problem described in the spec.
    // n is the number of gates/keycards.
    // gates is the clearance level of each gate, read from top to bottom
    // keycards is the clearance level of each keycard, read from top to bottom
    // You should return an n length array where the ith index tells us how many gates could be opened from the ith room (top to bottom).

    // In the example in the spec, the input would be:
    // n: 8
    // gates: [5, 2, 3, 2, 2, 3, 1, 2]
    // keycards: [1, 4, 1, 1, 3, 2, 0, 2]
    // And your function should return:
    // [0, 1, 0, 0, 4, 0, 0, 2]
    int* result = new int[n];

    Monotonic_stack* stack = new Monotonic_stack();

    for (int i = 0; i < n; i++)
    {
        result[i] = stack->push(new Block(i, gates,keycards));
    }

    delete stack;
    return result;
}

int ontrack_test() {
    int n;
    int* gates;
    int* keycards;
    cout << "Enter n: " << endl;
    cin >> n;
    gates = new int[n];
    keycards = new int[n];
    cout << "Enter gates: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> gates[i];
    }
    cout << "Enter keycards: " << endl;
    for (int i = 0; i < n; i++) {
        cin >> keycards[i];
    }
    if (!cin) {
        cout << "ERROR: could not read input data" << endl;
        delete[] gates;
        delete[] keycards;
        return 1;
    }

    int* result = solve(n, gates, keycards);
    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    delete[] gates;
    delete[] keycards;
    delete[] result;
    return 0;
}

int main(int argc, char** argv) {
    if (argc != 1) {
        return ontrack_test();
    }
    int n = 8;
    int* gates = new int[] {5, 2, 3, 2, 2, 3, 1, 2};
    int* keycards =  new int[] {1, 4, 1, 1, 3, 2, 0, 2};
    int* result = solve(n, gates, keycards);
    for (int i = 0; i < n; i++) {
        cout << result[i] << " ";
    }
    cout << endl;
    delete[] gates;
    delete[] keycards;
    delete[] result;
    return 0;
}
