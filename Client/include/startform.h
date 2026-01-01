#pragma once
#include "form.h"

#include <array>
#include <mutex>
#include <vector>
#include <memory>

namespace Forms {
    
class StartForm final : public Form {
public:
    StartForm();
    ~StartForm();

protected:
    void onCreate() override;
    void onDraw() const override;

    void onLeftButtonPress(Vector2i mousePosition) override;
    void onChar(char symbol) override;

private:
    std::vector<Controls::Label> vLabel;
    std::vector<Controls::Choice> vChoice;
    std::vector<Controls::Input> vInput;
    Controls::Button startB, exitB;

    //std::unique_ptr<EngineForm> engineForm;
    //std::unique_ptr<PvpForm> pvpForm;
    bool pvp{ false };
    bool turn{ true };
};

} // namespace Forms