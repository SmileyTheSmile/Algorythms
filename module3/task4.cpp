#include <assert.h>
#include <array>
#include <cstring>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_set>


#define SIDESIZE 4
#define FIELDSIZE 16


class State {
public:
    State(const std::array<char, FIELDSIZE>& _gameField);

    void newHValue();

    std::string getHistory() const {
        return history;
    }

    int getHValue() const {
        return hValue;
    }

    bool operator<(const State& state) const {
        return hValue < state.getHValue();
    }

    bool operator==(const State& state) const {
        return gameField == state.gameField;
    }
    bool operator!=(const State& state) const {
        return !(*this == state);
    }

    bool CanGoLeft() const;
    bool CanGoUp() const;
    bool CanGoRight() const;
    bool CanGoDown() const;
    bool isSolved() const;
    bool canSolve() const;

    int manhDist() const;
    int lastGo() const;

    State Move() const;
    State GoLeft() const;
    State GoUp() const;
    State GoRight() const;
    State GoDown() const;

private:
    int hValue;
    std::array<char, FIELDSIZE> gameField;
    char positionZero;
    std::string history;

    friend struct StateHasher;
};

State::State(const std::array<char, FIELDSIZE> &_gameField)
    : gameField(_gameField), positionZero(-1), history("")
{
    for (unsigned int i = 0; i < gameField.size(); ++i)
        if (gameField[i] == 0)
        {
            positionZero = i;
            break;
        }
    newHValue();
    assert(positionZero != -1);
}

bool State::CanGoLeft() const {
    return positionZero % SIDESIZE != 0;
}

bool State::CanGoUp() const {
    return positionZero >= SIDESIZE;
}

bool State::CanGoRight() const {
    return positionZero % SIDESIZE < SIDESIZE - 1;
}

bool State::CanGoDown() const {
    return positionZero < FIELDSIZE - SIDESIZE;
}

State State::Move() const
{
    assert(CanGoUp());
    State newState(*this);
    std::swap(newState.gameField[newState.positionZero],
        newState.gameField[newState.positionZero - SIDESIZE]);
    newState.positionZero -= SIDESIZE;
    newState.history += 'D';
    newState.newHValue();
    return newState;
}

State State::GoLeft() const
{
    assert(CanGoLeft());
    State newState(*this);
    std::swap(newState.gameField[newState.positionZero],
        newState.gameField[newState.positionZero - 1]);
    --newState.positionZero;
    newState.history += 'R';
    newState.newHValue();

    return newState;
}

State State::GoUp() const
{
    assert(CanGoUp());
    State newState(*this);
    std::swap(newState.gameField[newState.positionZero],
        newState.gameField[newState.positionZero - SIDESIZE]);
    newState.positionZero -= SIDESIZE;
    newState.history += 'D';
    newState.newHValue();
    return newState;
}

State State::GoRight() const
{
    assert(CanGoRight());
    State newState(*this);
    std::swap(newState.gameField[newState.positionZero],
        newState.gameField[newState.positionZero + 1]);
    ++newState.positionZero;
    newState.history += 'L';
    newState.newHValue();
    return newState;
}

State State::GoDown() const
{
    assert(CanGoDown());
    State newState(*this);
    std::swap(newState.gameField[newState.positionZero],
        newState.gameField[newState.positionZero + SIDESIZE]);
    newState.positionZero += SIDESIZE;
    newState.history += 'U';
    newState.newHValue();
    return newState;
}

struct StateHasher {
    size_t operator()(const State& state) const {
        size_t hash = 0;
        memcpy(&hash, &state.gameField[0], sizeof(hash));
        return hash;
    }
};

void State::newHValue() {
    hValue = manhDist() + lastGo();
}

int State::manhDist() const {
    int distance = 0;
    for (int i = 0; i < FIELDSIZE; i++) {
        int value = (gameField[i] + FIELDSIZE - 1) % FIELDSIZE;
        distance += abs((value - i) % SIDESIZE) +
                    abs((value / SIDESIZE - i / SIDESIZE)); 
    }

    return distance;
}

bool State::isSolved() const {
    for (int i = 0; i < FIELDSIZE; i++)
        if (gameField[i] != (i + 1) % FIELDSIZE)
            return false;
    return true;
}

int State::lastGo() const {
    return (gameField[FIELDSIZE - 1] == FIELDSIZE - 1 || gameField[FIELDSIZE - 1] == FIELDSIZE - SIDESIZE) ? 0 : 2;
}

bool State::canSolve() const {
    int sum = 0;

    for (int i = FIELDSIZE - 1; i >= 0; i--) {
        if (gameField[i] == 0)
            continue;

        for (int j = i + 1; j < FIELDSIZE; ++j)
            if (gameField[i] > gameField[j] && gameField[j] != 0)
                sum++;
    }

    sum += (positionZero / SIDESIZE + 1);
    return sum % 2 == 0;
}

std::string GetResult(const State &firstState) {
    std::set<State> states;
    std::unordered_set<State, StateHasher> checked;

    states.insert(firstState);

    while (!states.empty())
    {
        State temp = *states.begin();

        states.erase(states.begin());

        if (temp.isSolved())
            return temp.getHistory();

        std::string history = temp.getHistory();

        if (temp.CanGoLeft()) {
            if (history.empty() || history.back() != 'L') {
                State newState = temp.GoLeft();
                if (checked.find(newState) == checked.end())
                {
                    checked.insert(newState);
                    states.insert(newState);
                }
            }
        }

        if (temp.CanGoUp() && (history.empty() || history.back() != 'U'))
        {
            State newState = temp.GoUp();
            if (checked.find(newState) == checked.end())
            {
                checked.insert(newState);
                states.insert(newState);
            }
        }

        if (temp.CanGoRight() && (history.empty() || history.back() != 'R'))
        {
            State newState = temp.GoRight();
            if (checked.find(newState) == checked.end())
            {
                checked.insert(newState);
                states.insert(newState);
            }
        }

        if (temp.CanGoDown() && (history.empty() || history.back() != 'D'))
        {
            State newState = temp.GoDown();
            if (checked.find(newState) == checked.end())
            {
                checked.insert(newState);
                states.insert(newState);
            }
        }
    }

    return std::string();
}


int main() {
    std::array<char, FIELDSIZE> gameField;
    for (int i = 0; i < FIELDSIZE; i++) {
        int a = 0;
        std::cin >> a;
        gameField[i] = a;
    }

    State state(gameField);
    if (!state.canSolve()) {
        std::cout << "-1";
        return 0;
    }

    std::string result = GetResult(state);
    std::cout << result.size() << std::endl;

    for (char i : result)
        std::cout << i << " ";

    return 0;
}