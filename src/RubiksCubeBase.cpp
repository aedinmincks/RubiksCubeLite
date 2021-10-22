#include "RubiksCubeBase.h"

CRubiksCubeBase::CRubiksCubeBase(int n)
{
    size = n;
    state.assign(n, 0);
    goalstate.assign(n, 0);
}

bool CRubiksCubeBase::SetState(vi v)
{
    if (v.size() == size)
    {
        state = v;
        return true;
    }

    return false;
}

void CRubiksCubeBase::DoMove(int move)
{
    state = ApplyMove(move, state);
}

vi CRubiksCubeBase::ApplyMove(int move, vi v)
{
    return vi();
}
