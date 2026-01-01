#pragma once
#include "form.h"

namespace Forms {
    
class AnalysicsForm final : public Form {
public:
    AnalysicsForm(std::vector<AssessMoveData>& data);

protected:
    void onDraw() override;

    //void onLeftButtonPress(Vector2i mousePosition) override;

private:
    Controls::Button exitB;
    Controls::Board board;
    //Controls::AssessBar bar;
    Controls::Button flipB;
    //Controls::ProgressBar pbar;
    AnalysicsController control;
    //Controls::CommentSection section;
};

} // namespace Forms