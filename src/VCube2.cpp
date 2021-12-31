#include "VCube2.h"
#include <chrono>
#include <map>
#include <queue>

std::vector<std::string> VCube2::goal = {"UFR", "URB", "UBL", "ULF", "DRF", "DFL", "DLB", "DBR"};

VCube2::VCube2() : CRubiksCubeBase(16)
{
    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

VCube2::VCube2(const vi &state) : CRubiksCubeBase(16)
{
    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;
    }

    SetState(state);
}

void VCube2::Reset()
{
    currentState.assign(16, 0);
    goalState.assign(16, 0);

    for (int i = 0; i < 8; i++)
    {
        goalState[i] = i;
        currentState[i] = i;
    }
}

void VCube2::Show()
{
    const static std::vector<int> faces = {3,  9,  12, 1,  13, 16, 0,  17, 4,  2,  5,  8,
                                           21, 14, 11, 20, 10, 7,  22, 6,  19, 23, 18, 15};

    std::vector<char> v(24, ' ');
    int n = 0;

    for (int i = 0; i < 8; i++)
    {
        int index = currentState[i];
        int dir = currentState[i + 8];

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

    printf("  %c%c\r\n", v[0], v[1]);
    printf("  %c%c\r\n", v[2], v[3]);
    printf("%c%c%c%c%c%c%c%c\r\n", v[4], v[5], v[8], v[9], v[12], v[13], v[16], v[17]);
    printf("%c%c%c%c%c%c%c%c\r\n", v[6], v[7], v[10], v[11], v[14], v[15], v[18], v[19]);
    printf("  %c%c\r\n", v[20], v[21]);
    printf("  %c%c\r\n", v[22], v[23]);
}

vi VCube2::ApplyMove(int move, vi state)
{
    const static std::vector<std::vector<int>> affectedCubiesLocation = {
        {0, 1, 2, 3}, // U
        {4, 5, 6, 7}, // D
        {0, 3, 5, 4}, // F
        {2, 1, 7, 6}, // B
        {3, 2, 6, 5}, // L
        {1, 0, 4, 7}, // R
    };

    const static std::vector<std::vector<int>> affectedCubiesOrientation = {
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
            int target = affectedCubiesLocation[face][i];
            int killer = affectedCubiesLocation[face][(i == 3) ? 0 : i + 1];
            int orientationDelta = affectedCubiesOrientation[face][i];

            state[target] = oldstate[killer];
            state[target + 8] = oldstate[killer + 8] + orientationDelta;
            if (!turns)
            {
                state[target + 8] %= 3;
            }
        }
    }

    return state;
}

const static std::vector<std::string> MovesString = {
    "U", "U2", "U'", "D", "D2", "D'", "F", "F2", "F'", "B", "B2", "B'", "L", "L2", "L'", "R", "R2", "R'",
};

std::string VCube2::Solve()
{
    const static std::vector<std::vector<int>> applicableMoves = {
        {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17},
        {0, 1, 2, 3, 4, 5, 7, 10, 13, 16},
        {1, 4, 7, 10, 13, 16},
    };

    std::string ans;

    int phase = 0;
    vi state = currentState;

    while (phase < 3)
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

std::string VCube2::GetRegex()
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

int VCube2::GetMove(std::string s)
{
    auto it = std::find(MovesString.begin(), MovesString.end(), s);

    if (it == MovesString.end())
    {
        return 0;
    }

    return it - MovesString.begin();
}

std::string VCube2::Random(int n)
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

vi VCube2::id(vi state, int phase)
{
    switch (phase)
    {
    case 0: //(U D L2 R2 F2 B2)
    {
        return vi(state.begin() + 8, state.begin() + 16);
    }
    case 1: //(U2 D2 L2 R2 F2 B2)
    {
        vi result(9);
        for (int c = 0; c < 8; c++)
        {
            result[c] = state[c] & 5;
        }
        for (int i = 0; i < 8; i++)
        {
            for (int j = i + 1; j < 8; j++)
            {
                result[8] ^= state[i] > state[j];
            }
        }
        return result;
    }
    case 2:
    default:
        break;
    }

    return state;
}

bool VCube2::State2String(const vi &state, std::vector<std::string> &s)
{
    std::vector<std::string> ans(8);

    for (int i = 0; i < 8; i++)
    {
        int index = currentState[i];
        int dir = currentState[i + 8];

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

bool VCube2::String2State(const std::vector<std::string> &s, vi &state)
{
    vi ans(16);

    if (s.size() != 8)
    {
        return false;
    }

    for (int i = 0; i < 8; i++)
    {
        std::string cubie = s[i];
        int j;
        for (j = 0; j < 3; j++)
        {
            auto it = std::find(goal.begin(), goal.end(), cubie);
            if (it != goal.end())
            {
                ans[i] = it - goal.begin();
                ans[i + 8] = j;
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

int VCube2::inverse(int move)
{
    return move + 2 - 2 * (move % 3);
}
