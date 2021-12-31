
#include "VCube3.h"
#include <algorithm>
#include <chrono>
#include <queue>
#include <map>

std::vector<std::string> VCube3::goal = {"UF", "UR", "UB",  "UL",  "DF",  "DR",  "DB",  "DL",  "FR",  "FL",
                                         "BR", "BL", "UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR"};
VCube3::VCube3() : CRubiksCubeBase(40)
{
    for (int i = 0; i < 20; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

VCube3::VCube3(const vi &state) : CRubiksCubeBase(40)
{
    for (int i = 0; i < 20; i++)
    {
        goalState[i] = i;
    }

    SetState(state);
}

void VCube3::Reset()
{
    currentState.assign(40, 0);
    goalState.assign(40, 0);

    for (int i = 0; i < 20; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

void VCube3::Show()
{
    const static std::vector<int> faces = {6,  12, 4,  15, 1,  18, 3,  9,  41, 32, 44, 35, 46, 38, 43, 29,
                                           23, 24, 22, 21, 26, 25, 27, 20, 7,  13, 14, 2,  16, 17, 0,  19,
                                           8,  5,  10, 11, 42, 34, 33, 40, 31, 30, 45, 28, 39, 47, 37, 36};

    std::vector<char> v(48, ' ');
    int n = 0;

    for (int i = 0; i < 20; i++)
    {
        int index = currentState[i];
        int dir = currentState[i + 20];

        std::string cubie = goal[index];
        while (dir--)
        {
            cubie = cubie.back() +cubie.substr(0, cubie.size() - 1);
        }

        for (int j = 0; j < cubie.size(); j++)
        {
            v[faces[n++]] = cubie[j];
        }
    }

    printf("   %c%c%c\r\n", v[0], v[1], v[2]);
    printf("   %c%c%c\r\n", v[3], 'U', v[4]);
    printf("   %c%c%c\r\n", v[5], v[6], v[7]);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c\r\n", v[8], v[9], v[10], v[11], v[12], v[13], v[14], v[15], v[16], v[17], v[18],
           v[19]);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c\r\n", v[20], 'L', v[21], v[22], 'F', v[23], v[24], 'R', v[25], v[26], 'B', v[27]);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c\r\n", v[28], v[29], v[30], v[31], v[32], v[33], v[34], v[35], v[36], v[37], v[38],
           v[39]);
    printf("   %c%c%c\r\n", v[40], v[41], v[42]);
    printf("   %c%c%c\r\n", v[43], 'D', v[44]);
    printf("   %c%c%c\r\n", v[45], v[46], v[47]);
}

vi VCube3::ApplyMove(int move, vi state)
{
    const static std::vector<std::vector<int>> affectedCubiesSideLocation = {
        {0, 1, 2, 3},   // U
        {4, 5, 6, 7},   // D
        {0, 9, 4, 8},   // F
        {2, 10, 6, 11}, // B
        {3, 11, 7, 9},  // L
        {1, 8, 5, 10},  // R
    };

    const static std::vector<std::vector<int>> affectedCubiesSideOrientation = {
        {0, 0, 0, 0}, // U
        {0, 0, 0, 0}, // D
        {1, 1, 1, 1}, // F
        {1, 1, 1, 1}, // B
        {0, 0, 0, 0}, // L
        {0, 0, 0, 0}, // R
    };

    const static std::vector<std::vector<int>> affectedCubiesCornerLocation = {
        {12, 13, 14, 15}, // U
        {16, 17, 18, 19}, // D
        {12, 15, 17, 16}, // F
        {14, 13, 19, 18}, // B
        {15, 14, 18, 17}, // L
        {13, 12, 16, 19}, // R
    };

    const static std::vector<std::vector<int>> affectedCubiesCornerOrientation = {
        {0, 0, 0, 0}, // U
        {0, 0, 0, 0}, // D
        {2, 1, 2, 1}, // F
        {2, 1, 2, 1}, // B
        {2, 1, 2, 1}, // L
        {2, 1, 2, 1}, // R
    };

    int turns = move % 3 + 1;
    int face = move / 3;

    while (turns--)
    {
        vi oldstate = state;
        for (int i = 0; i < 4; i++)
        {
            int target = affectedCubiesSideLocation[face][i];
            int killer = affectedCubiesSideLocation[face][(i == 3) ? 0 : i + 1];
            int orientationDelta = affectedCubiesSideOrientation[face][i];

            state[target] = oldstate[killer];
            state[target + 20] = oldstate[killer + 20] + orientationDelta;
            if (!turns)
            {
                state[target + 20] %= 2;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            int target = affectedCubiesCornerLocation[face][i];
            int killer = affectedCubiesCornerLocation[face][(i == 3) ? 0 : i + 1];
            int orientationDelta = affectedCubiesCornerOrientation[face][i];

            state[target] = oldstate[killer];
            state[target + 20] = oldstate[killer + 20] + orientationDelta;
            if (!turns)
            {
                state[target + 20] %= 3;
            }
        }
    }

    return state;
}

const static std::vector<std::string> MovesString = {
    "U", "U2", "U'", "D", "D2", "D'", "F", "F2", "F'", "B", "B2", "B'", "L", "L2", "L'", "R", "R2", "R'",
};

std::string VCube3::Solve()
{
    const static std::vector<std::vector<int>> applicableMoves = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
        {0, 1, 2, 3, 4, 5, 7, 10, 12, 13, 14, 15, 16, 17},
        {0, 1, 2, 3, 4, 5, 7, 10, 13, 16},
        {1, 4, 7, 10, 13, 16},
    };

    std::string ans;

    int phase = 0;
    vi state = currentState;

    while (phase < 4)
    {
        vi currentId = id(state, phase), goalId = id(goalState, phase);
        if (currentId == goalId)
        {
            phase++;
            continue;
        }

        std::queue<vi> q;
        q.push(state);
        q.push(goalState);

        std::map<vi, vi> predecessor;
        std::map<vi, int> direction, lastMove;
        direction[currentId] = 1;
        direction[goalId] = 2;

        while (1)
        {
            vi oldState = q.front();
            q.pop();
            vi oldId = id(oldState, phase);
            int &oldDir = direction[oldId];

            for (int move : applicableMoves[phase])
            {
                vi newState = ApplyMove(move, oldState);
                vi newId = id(newState, phase);
                int &newDir = direction[newId];

                if (newDir && newDir != oldDir)
                {
                    if (oldDir > 1)
                    {
                        swap(newId, oldId);
                        move = inverse(move);
                    }

                    vi algorithm(1, move);
                    while (oldId != currentId)
                    {
                        algorithm.insert(algorithm.begin(), lastMove[oldId]);
                        oldId = predecessor[oldId];
                    }
                    while (newId != goalId)
                    {
                        algorithm.push_back(inverse(lastMove[newId]));
                        newId = predecessor[newId];
                    }

                    for (int i = 0; i < (int)algorithm.size(); i++)
                    {
                        ans += MovesString[algorithm[i]];
                        state = ApplyMove(algorithm[i], state);
                    }

                    goto nextPhasePlease;
                }

                if (!newDir)
                {
                    q.push(newState);
                    newDir = oldDir;
                    lastMove[newId] = move;
                    predecessor[newId] = oldId;
                }
            }
        }

    nextPhasePlease:
        phase++;
    }

    return ans;
}

std::string VCube3::GetRegex()
{
    std::vector<std::string> v(MovesString);

    std::sort(v.begin(), v.end(), [](std::string &a, std::string &b) { return a.size() > b.size(); });

    std::string ans;

    for (auto &s : v)
    {
        ans += s;
        ans += '|';
    }

    if (!ans.empty())
    {
        ans.pop_back();
    }

    return ans;
}

int VCube3::GetMove(std::string s)
{
    auto it = std::find(MovesString.begin(), MovesString.end(), s);

    if (it == MovesString.end())
    {
        return 0;
    }

    return it - MovesString.begin();
}

std::string VCube3::Random(int n)
{
    std::string ans;

    std::srand(std::time(NULL));
    vi state = currentState;

    for (int i = 0; i < n; i++)
    {
        int move = std::rand() % MovesString.size();

        state = ApplyMove(move, state);

        ans += MovesString[move];
    }

    SetState(state);

    return ans;
}

bool VCube3::State2String(const vi &state, std::vector<std::string> &s)
{
    std::vector<std::string> ans(20);

    for (int i = 0; i < 20; i++)
    {
        int index = currentState[i];
        int dir = currentState[i + 20];

        if (index < 0 || index >= goal.size())
        {
            return false;
        }

        std::string cubie = goal[index];
        while (dir--)
        {
            cubie = cubie.substr(1) + cubie[0];
        }

        ans[i] = cubie;
    }

    s = ans;

    return true;
}

bool VCube3::String2State(const std::vector<std::string> &s, vi &state)
{
    vi ans(40);

    if (s.size() != 20)
    {
        return false;
    }

    for (int i = 0; i < 12; i++)
    {
        std::string cubie = s[i];
        int j;
        for (j = 0; j < 2; j++)
        {
            auto it = std::find(goal.begin(), goal.end(), cubie);
            if (it != goal.end())
            {
                ans[i] = it - goal.begin();
                ans[i + 20] = j;
                break;
            }
            else
            {
                cubie = cubie.substr(1) + cubie[0];
            }
        }
        if (j == 2)
        {
            return false;
        }
    }

    for (int i = 12; i < 20; i++)
    {
        std::string cubie = s[i];
        int j;
        for (j = 0; j < 3; j++)
        {
            auto it = std::find(goal.begin(), goal.end(), cubie);
            if (it != goal.end())
            {
                ans[i] = it - goal.begin();
                ans[i + 20] = j;
                break;
            }
            else
            {
                cubie = cubie.substr(1) + cubie[0];
            }
        }
        if (j == 3)
        {
            return false;
        }
    }

    state = ans;

    return true;
}

vi VCube3::id(vi state, int phase)
{
    switch (phase)
    {
    case 0: //(U D L R F2 B2)
    {
        return vi(state.begin() + 20, state.begin() + 32);
    }
    case 1: //(U D L2 R2 F2 B2)
    {
        vi result(state.begin() + 31, state.begin() + 40);
        for (int e = 0; e < 12; e++)
            result[0] |= (state[e] / 8) << e;
        return result;
    }
    case 2: //(U2 D2 L2 R2 F2 B2)
    {
        vi result(21);
        for (int c = 0; c < 12; c++)
        {
            result[c] = (state[c] > 7) ? 2 : (state[c] & 1);
        }
        for (int c = 12; c < 20; c++)
        {
            result[c] = (state[c] - 12) & 5;
        }
        for (int i = 12; i < 20; i++)
        {
            for (int j = i + 1; j < 20; j++)
            {
                result[20] ^= state[i] > state[j];
            }
        }
        return result;
    }
    case 3:
    default:
        break;
    }

    return state;
}

int VCube3::inverse(int move)
{
    return move + 2 - 2 * (move % 3);
}
