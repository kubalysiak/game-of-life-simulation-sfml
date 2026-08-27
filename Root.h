#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>
#include <filesystem> 
#include <string>

#include "Board.h"
#include "Button.h"
#include "TextField.h"
#include "Label.h"
#include "Preset.h" 

enum class AppState {
    Menu,
    Simulation
};

class Root {
public:
    Root();
    void setup();
    void run();

private:
    void handleEvents();
    void handleMenuEvents(const sf::Event& event);
    void handleSimulationEvents(const sf::Event& event);
    void update();
    void render();

    
    void setActiveSpeed(int index); 

    void refreshPresetList();
    // Window config
    const int _WINDOW_WIDTH = 1400;
    const int _WINDOW_HEIGHT = 900;
    const std::string _WINDOW_NAME = "Game of Life - kubalysiak";

    sf::RenderWindow _mainWindow;
    AppState _currentState;
    sf::Font _mainFont;
    sf::Texture _menuBgTex;
    sf::Sprite _menuBgSprite;
    sf::Texture _panelBgTex;
    // Viewports for split-screen rendering
    sf::View _uiView;
    sf::View _boardView;
    sf::View _presetListView;
    sf::RectangleShape _uiPanelBackground;
    // Camera and interaction states
    bool _isPanning;
    sf::Vector2i _panningAnchor;
    bool _blockDrawing;
    // Simulation state
    bool _isSimulationRunning;
    sf::Clock _simClock;
    sf::Time _simInterval;

    std::unique_ptr<Board> _board;

    // UI - Menu
    std::unique_ptr<Label> _menuTitle;
    std::unique_ptr<TextField> _sizeInput;
    std::unique_ptr<Button> _confirmButton;
    std::unique_ptr<Label> _errorLabel;
 

    // UI - Simulation
    std::unique_ptr<Label> _uiTitleLabel;
    std::unique_ptr<Label> _authorLabel;
    std::unique_ptr<Label> _legendLabel;

    std::unique_ptr<Button> _startStopButton;
    Label* _startStopLabel = nullptr; //Observer pointer
 
    std::unique_ptr<Button> _clearButton;

    std::unique_ptr<Button> _speed05Btn;
    std::unique_ptr<Button> _speed1Btn;
    std::unique_ptr<Button> _speed2Btn;
    std::unique_ptr<Button> _speed4Btn;
   
    std::unique_ptr<Button> _randomizeButton;
    std::unique_ptr<TextField> _randomPercentInput;
    std::unique_ptr<Label> _randomPercentLabel;

    std::unique_ptr<Button> _wrapEdgesButton;
    Label* _wrapEdgesLabel = nullptr; //Observer pointer

 
    std::unique_ptr<Button> _backButton;
    // UI - Preset management
    std::unique_ptr<Button> _savePresetBtn;
    std::unique_ptr<TextField> _presetNameInput;
    std::unique_ptr<Label> _presetStatusLabel;
    std::unique_ptr<Button> _loadPresetBtn;

    sf::Clock _statusTimer;
    bool _isStatusTimerActive;

    bool _isPresetListOpen;
    int _selectedPresetIndex;

    std::vector<Preset> _presets;
    std::vector<std::unique_ptr<Button>> _presetButtons;
    sf::RectangleShape _presetListBg;
};