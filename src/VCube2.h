#pragma once

#include "RubiksCubeBase.h"
#include "string"

class VCube2 : public CRubiksCubeBase
{
  public:
    VCube2();
    VCube2(std::vector<std::string> cubies);

    virtual void Reset() override;

    virtual void Show() override;

    virtual vi ApplyMove(int move, vi state) override;

    virtual std::string Solve() override;

    virtual std::string GetRegex() override;

    virtual int GetMove(std::string s) override;

  private:
    static std::vector<std::string> goal;

    static vi id(vi state, int phase);

    static int inverse(int move);
};
