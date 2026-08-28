#include "Root.h"
#include <iostream>
#include <format>

Root::Root()
    : _currentState(AppState::Menu),
    _isSimulationRunning(false),
    _isPanning(false),
    _blockDrawing(false),
    _simInterval(sf::seconds(0.1f)),
    _isPresetListOpen(false),
    _selectedPresetIndex(-1),
    _isStatusTimerActive(false)
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 4;

    _mainWindow.create(
        sf::VideoMode(_WINDOW_WIDTH, _WINDOW_HEIGHT),
        _WINDOW_NAME,
        sf::Style::Close,
        settings
    );

    _mainWindow.setFramerateLimit(60);
}

void Root::setup() {
    if (!_mainFont.loadFromFile("assets/ARIAL.TTF")) {
        std::cerr << "[ERROR] Failed to load font. Make sure ARIAL.TTF is in the assets folder." << std::endl;
    }

    if (_menuBgTex.loadFromFile("assets/menu_bg.png")) _menuBgSprite.setTexture(_menuBgTex);

    _uiPanelBackground.setSize(sf::Vector2f(400, 900));
    _uiPanelBackground.setPosition(1000, 0);
    
    if (_panelBgTex.loadFromFile("assets/panel_bg.png")) {
        _uiPanelBackground.setFillColor(sf::Color::White);
        _uiPanelBackground.setTexture(&_panelBgTex);
    }
    else {
        _uiPanelBackground.setFillColor(sf::Color(25, 25, 30));
    }

    _uiView = sf::View(sf::FloatRect(0.f, 0.f, static_cast<float>(_WINDOW_WIDTH), static_cast<float>(_WINDOW_HEIGHT)));
    _boardView = sf::View(sf::FloatRect(0, 0, 1000, 900));
    _boardView.setViewport(sf::FloatRect(0.f, 0.f, 1000.f / 1400.f, 1.f));

    // Main menu setup
    _menuTitle = std::make_unique<Label>(700.f, 350.f, "ENTER BOARD SIZE (10-300):", 34, _mainFont);
    _menuTitle->centerOrigin();

    _sizeInput = std::make_unique<TextField>(650.f, 410.f, 100.f, 45.f, "100", 30, _mainFont);

    _confirmButton = std::make_unique<Button>(600.f, 480.f, 200.f, 60.f);
    _confirmButton->setBaseColor(sf::Color(40, 120, 60));
    auto startLbl = std::make_unique<Label>(700.f, 510.f, "START", 28, _mainFont);
    startLbl->centerOrigin();
    _confirmButton->addElement(std::move(startLbl));

    _errorLabel = std::make_unique<Label>(700.f, 580.f, "", 24, _mainFont, sf::Color(255, 100, 100));
    _errorLabel->centerOrigin();

    // Simulation UI setup
    float centerUI = 1200;
    float uiX = 1040;

    _uiTitleLabel = std::make_unique<Label>(centerUI, 40.f, "GAME OF LIFE", 36, _mainFont, sf::Color(200, 200, 200));
    _uiTitleLabel->centerOrigin();
    _authorLabel = std::make_unique<Label>(centerUI, 75.f, "by kubalysiak", 18, _mainFont, sf::Color(200, 200, 200));
    _authorLabel->centerOrigin();

    _startStopButton = std::make_unique<Button>(uiX, 130.f, 320.f, 60.f);
    _startStopButton->setBaseColor(sf::Color(40, 120, 60));
    auto startStopLbl = std::make_unique<Label>(centerUI, 160.f, "START", 28, _mainFont);
    startStopLbl->centerOrigin();
    _startStopLabel = startStopLbl.get();
    _startStopButton->addElement(std::move(startStopLbl));

    _clearButton = std::make_unique<Button>(uiX, 210.f, 320.f, 50.f);
    auto clrLbl = std::make_unique<Label>(centerUI, 235.f, "CLEAR", 24, _mainFont);
    clrLbl->centerOrigin();
    _clearButton->addElement(std::move(clrLbl));

    float sY = 290;
    _speed05Btn = std::make_unique<Button>(uiX, sY, 70.f, 45.f);
    _speed05Btn->addElement(std::make_unique<Label>(uiX + 35, sY + 12, "0.5x", 18, _mainFont));

    _speed1Btn = std::make_unique<Button>(uiX + 83, sY, 70.f, 45.f);
    _speed1Btn->addElement(std::make_unique<Label>(uiX + 83 + 35, sY + 12, "1x", 18, _mainFont));

    _speed2Btn = std::make_unique<Button>(uiX + 166, sY, 70.f, 45.f);
    _speed2Btn->addElement(std::make_unique<Label>(uiX + 166 + 35, sY + 12, "2x", 18, _mainFont));

    _speed4Btn = std::make_unique<Button>(uiX + 249, sY, 70.f, 45.f);
    _speed4Btn->addElement(std::make_unique<Label>(uiX + 249 + 35, sY + 12, "4x", 18, _mainFont));
    setActiveSpeed(1);

    _randomizeButton = std::make_unique<Button>(uiX, 400.f, 160.f, 45.f);
    auto rndLbl = std::make_unique<Label>(1120.f, 422.f, "RANDOMIZE", 20, _mainFont);
    rndLbl->centerOrigin();
    _randomizeButton->addElement(std::move(rndLbl));

    _randomPercentLabel = std::make_unique<Label>(1240.f, 420.f, "FILL %:", 16, _mainFont, sf::Color(150, 150, 150));
    _randomPercentLabel->centerOrigin();
    _randomPercentInput = std::make_unique<TextField>(1280.f, 407.f, 70.f, 30.f, "20", 20, _mainFont);

    _wrapEdgesButton = std::make_unique<Button>(uiX, 470.f, 320.f, 50.f);
    auto wrapLbl = std::make_unique<Label>(centerUI, 495.f, "EDGES: WALLS", 24, _mainFont);
    wrapLbl->centerOrigin();
    _wrapEdgesLabel = wrapLbl.get();
    _wrapEdgesButton->addElement(std::move(wrapLbl));

    _savePresetBtn = std::make_unique<Button>(uiX, 540.f, 120.f, 40.f);
    _savePresetBtn->setBaseColor(sf::Color(40, 80, 120));
    auto saveLbl = std::make_unique<Label>(uiX + 60, 560.f, "SAVE", 20, _mainFont);
    saveLbl->centerOrigin();
    _savePresetBtn->addElement(std::move(saveLbl));

    _presetNameInput = std::make_unique<TextField>(1180.f, 545.f, 180.f, 32.f, "my_pattern", 18, _mainFont);
    _presetStatusLabel = std::make_unique<Label>(1270.f, 530.f, "", 14, _mainFont, sf::Color::Yellow);
    _presetStatusLabel->centerOrigin();
    _loadPresetBtn = std::make_unique<Button>(uiX, 600.f, 320.f, 50.f);
    _loadPresetBtn->setBaseColor(sf::Color(120, 80, 40));
    auto loadLbl = std::make_unique<Label>(centerUI, 625.f, "LOAD PRESET", 24, _mainFont);
    loadLbl->centerOrigin();
    _loadPresetBtn->addElement(std::move(loadLbl));

    std::string legendText =
        "CONTROLS:\n"
        "LMB: Draw / Paste Preset\n"
        "RMB: Erase / Cancel Preset\n"
        "Hold Scroll: Pan camera\n"
        "Scroll: Zoom / Scroll List";
    _legendLabel = std::make_unique<Label>(centerUI, 730.f, legendText, 16, _mainFont, sf::Color(130, 130, 130));
    _legendLabel->centerOrigin();

    _backButton = std::make_unique<Button>(uiX, 810.f, 320.f, 50.f);
    _backButton->setBaseColor(sf::Color(120, 50, 50));
    auto backLbl = std::make_unique<Label>(centerUI, 835.f, "BACK TO MENU", 24, _mainFont);
    backLbl->centerOrigin();
    _backButton->addElement(std::move(backLbl));

    refreshPresetList();
}

void Root::refreshPresetList() {
    _presets.clear();
    _presetButtons.clear();
    std::filesystem::create_directory("presets");

    float listW = 380;
    float listH = 600;
    _presetListBg.setSize(sf::Vector2f(listW, listH));
    _presetListBg.setPosition(600, 150);
    _presetListBg.setFillColor(sf::Color(30, 30, 35, 240));
    _presetListBg.setOutlineThickness(2);
    _presetListBg.setOutlineColor(sf::Color(100, 100, 120));

    _presetListView.setViewport(sf::FloatRect(600.f / 1400.f, 150.f / 900.f, listW / 1400.f, listH / 900.f));
    _presetListView.setSize(listW, listH);
    _presetListView.setCenter(listW / 2.f, listH / 2.f);

    float btnY = 20;
    for (const auto& entry : std::filesystem::directory_iterator("presets")) {
        if (entry.path().extension() == ".txt") {
            Preset p = PresetManager::loadPreset(entry.path().string());
            if (p.width > 0) {
                _presets.push_back(p);
                auto btn = std::make_unique<Button>(20.f, btnY, listW - 40, 50.f);
                btn->setBaseColor(sf::Color(60, 60, 70));
                std::string presetLabel = std::format("{} ({}x{})", p.name, p.width, p.height);
                auto lbl = std::make_unique<Label>(listW / 2.f, btnY + 25, presetLabel, 20, _mainFont);
                lbl->centerOrigin();
                btn->addElement(std::move(lbl));

                _presetButtons.push_back(std::move(btn));
                btnY += 60;
            }
        }
    }
}

void Root::run() {
    setup();
    while (_mainWindow.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Root::handleEvents() {
    sf::Event event;
    while (_mainWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) _mainWindow.close();
        if (_currentState == AppState::Menu) handleMenuEvents(event);
        else if (_currentState == AppState::Simulation) handleSimulationEvents(event);
    }
}

void Root::handleMenuEvents(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow));
        _sizeInput->setFocus(_sizeInput->getGlobalBounds().contains(mousePos));

        if (_confirmButton->getGlobalBounds().contains(mousePos)) {
            std::string input = _sizeInput->getString();
            if (!input.empty()) {
                int size = std::stoi(input);
                if (size >= 10 && size <= 300) {
                    _errorLabel->setString("");
                    _board = std::make_unique<Board>(size, size, 20);
                    float boardPixels = size * 20.f;
                    _boardView.setCenter(boardPixels / 2.f, boardPixels / 2.f);
                    _currentState = AppState::Simulation;
                }
                else {
                    _errorLabel->setString("ERROR: Size must be 10 - 300!");
                }
            }
        }
    }

    if (event.type == sf::Event::TextEntered && _sizeInput->isFocused()) {
        if (event.text.unicode == 8) {
            std::string str = _sizeInput->getString();
            _isStatusTimerActive = false;
            if (!str.empty()) { str.pop_back(); _sizeInput->setString(str); }
        }
        else if (event.text.unicode >= 48 && event.text.unicode <= 57) {
            if (_sizeInput->getString().length() < 3) {
                _sizeInput->setString(_sizeInput->getString() + static_cast<char>(event.text.unicode));
            }
        }
    }
}

void Root::handleSimulationEvents(const sf::Event& event) {
    
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left) {
        _blockDrawing = false;
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Middle) {
        _isPanning = true;
        _panningAnchor = sf::Mouse::getPosition(_mainWindow);
    }
    if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Middle) {
        _isPanning = false;
    }
    if (event.type == sf::Event::MouseMoved && _isPanning) {
        sf::Vector2i currentPos = sf::Mouse::getPosition(_mainWindow);
        sf::Vector2f oldPos = _mainWindow.mapPixelToCoords(_panningAnchor, _boardView);
        sf::Vector2f newPos = _mainWindow.mapPixelToCoords(currentPos, _boardView);
        _boardView.move(oldPos - newPos);
        _panningAnchor = currentPos;
    }

    if (event.type == sf::Event::MouseWheelScrolled) {
        sf::Vector2f uiMousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _uiView);
        if (_isPresetListOpen && _presetListBg.getGlobalBounds().contains(uiMousePos)) {
            _presetListView.move(0, -event.mouseWheelScroll.delta * 40.f);
        }
        else if (sf::Mouse::getPosition(_mainWindow).x < 1000) {
            if (event.mouseWheelScroll.delta > 0) _boardView.zoom(0.85f);
            else _boardView.zoom(1.15f);
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f uiMousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _uiView);

        if (_isPresetListOpen) {
            if (_presetListBg.getGlobalBounds().contains(uiMousePos)) {
                sf::Vector2f listMousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _presetListView);
                for (size_t i = 0; i < _presetButtons.size(); ++i) {
                    if (_presetButtons[i]->getGlobalBounds().contains(listMousePos)) {
                        _selectedPresetIndex = static_cast<int>(i);
                        _isPresetListOpen = false;
                        break;
                    }
                }
            }
            else {
                _isPresetListOpen = false;
            }
            return;
        }

        _randomPercentInput->setFocus(_randomPercentInput->getGlobalBounds().contains(uiMousePos));
        _presetNameInput->setFocus(_presetNameInput->getGlobalBounds().contains(uiMousePos));

        if (_startStopButton->getGlobalBounds().contains(uiMousePos)) {
            _isSimulationRunning = !_isSimulationRunning;
            if (_isSimulationRunning) {
                _startStopLabel->setString("PAUSE");
                _startStopButton->setBaseColor(sf::Color(150, 50, 50));
            }
            else {
                _startStopLabel->setString("START");
                _startStopButton->setBaseColor(sf::Color(40, 120, 60));
            }
        }
        else if (_clearButton->getGlobalBounds().contains(uiMousePos)) {
            _board->clearBoard();
            _isSimulationRunning = false;
            _startStopLabel->setString("START");
            _startStopButton->setBaseColor(sf::Color(40, 120, 60));
        }
        else if (_randomizeButton->getGlobalBounds().contains(uiMousePos)) {
            int pct = 20;
            if (!_randomPercentInput->getString().empty()) pct = std::stoi(_randomPercentInput->getString());
            _board->randomizeBoard(pct);
        }
        else if (_wrapEdgesButton->getGlobalBounds().contains(uiMousePos)) {
            _board->setWrapEdges(!_board->getWrapEdges());
            if (_board->getWrapEdges()) _wrapEdgesLabel->setString("EDGES: WRAP");
            else _wrapEdgesLabel->setString("EDGES: WALLS");
        }
        else if (_speed05Btn->getGlobalBounds().contains(uiMousePos)) {
            _simInterval = sf::seconds(0.2f);
            setActiveSpeed(0);
        }
        else if (_speed1Btn->getGlobalBounds().contains(uiMousePos)) {
            _simInterval = sf::seconds(0.1f);
            setActiveSpeed(1);
        }
        else if (_speed2Btn->getGlobalBounds().contains(uiMousePos)) {
            _simInterval = sf::seconds(0.05f);
            setActiveSpeed(2);
        }
        else if (_speed4Btn->getGlobalBounds().contains(uiMousePos)) {
            _simInterval = sf::seconds(0.01f);
            setActiveSpeed(3);
        }
        else if (_savePresetBtn->getGlobalBounds().contains(uiMousePos)) {
            std::string name = _presetNameInput->getString();
            if (!name.empty()) {
                if (std::filesystem::exists("presets/" + name + ".txt")) {
                    
                    _presetStatusLabel->setString("File already exists!");
                    _presetStatusLabel->setColor(sf::Color(255, 100, 100)); 
                    _presetStatusLabel->centerOrigin(); 

                    _isStatusTimerActive = true;
                    _statusTimer.restart();
                }
                else {
                    Preset p = _board->extractPreset(name);
                    if (p.width > 0) {
                        PresetManager::savePreset(p, "presets");
                        refreshPresetList();

                        _presetStatusLabel->setString("Saved successfully!");
                        _presetStatusLabel->setColor(sf::Color(100, 255, 100)); 
                        _presetStatusLabel->centerOrigin();
                        _presetNameInput->setString(""); 
                        _isStatusTimerActive = true;
                        _statusTimer.restart();
                    }
                }
            }
        }
        else if (_loadPresetBtn->getGlobalBounds().contains(uiMousePos)) {
            _isPresetListOpen = true;
            _isSimulationRunning = false;
            _startStopLabel->setString("START");
            _startStopButton->setBaseColor(sf::Color(40, 120, 60));
        }
        else if (_backButton->getGlobalBounds().contains(uiMousePos)) {
            _currentState = AppState::Menu;
            _errorLabel->setString("");
            _isSimulationRunning = false;
            _selectedPresetIndex = -1;
            _board->clearGhost();
            _startStopLabel->setString("START");
            _startStopButton->setBaseColor(sf::Color(40, 120, 60));
            setActiveSpeed(1);
            _simInterval = sf::seconds(0.1f);
        }
        else if (_selectedPresetIndex != -1 && sf::Mouse::getPosition(_mainWindow).x < 1000) {
            _board->pasteGhost();
            _selectedPresetIndex = -1;
            _board->clearGhost();
            _blockDrawing = true; 
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
        if (_selectedPresetIndex != -1) {
            _selectedPresetIndex = -1;
            _board->clearGhost();
        }
    }

    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == 8) {
            if (_randomPercentInput->isFocused() && !_randomPercentInput->getString().empty()) {
                std::string str = _randomPercentInput->getString();
                str.pop_back(); _randomPercentInput->setString(str);
            }
            if (_presetNameInput->isFocused() && !_presetNameInput->getString().empty()) {
                std::string str = _presetNameInput->getString();
                str.pop_back();
                _presetNameInput->setString(str);
                _presetStatusLabel->setString(""); 
            }
        }
        else {
            if (_presetNameInput->isFocused() && event.text.unicode < 128) {
                char c = static_cast<char>(event.text.unicode);
                if (isalnum(c) || c == '_') {
                    _presetNameInput->setString(_presetNameInput->getString() + c);
                    _presetStatusLabel->setString(""); 
                    _isStatusTimerActive = false;
                }
            }
            if (_randomPercentInput->isFocused() && event.text.unicode >= 48 && event.text.unicode <= 57) {
                if (_randomPercentInput->getString().length() < 3) {
                    _randomPercentInput->setString(_randomPercentInput->getString() + static_cast<char>(event.text.unicode));
                }
            }
        }
    }
}

void Root::update() {
    bool hasFocus = _mainWindow.hasFocus(); 
    sf::Vector2f uiMousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _uiView);

    if (_currentState == AppState::Menu) {
        if (hasFocus) _confirmButton->updateHover(uiMousePos);
    }
    else if (_currentState == AppState::Simulation) {
        if (hasFocus) {
            _startStopButton->updateHover(uiMousePos);
            _clearButton->updateHover(uiMousePos);
            _randomizeButton->updateHover(uiMousePos);
            _wrapEdgesButton->updateHover(uiMousePos);
            _backButton->updateHover(uiMousePos);
            _savePresetBtn->updateHover(uiMousePos);
            _loadPresetBtn->updateHover(uiMousePos);

            _speed05Btn->updateHover(uiMousePos);
            _speed1Btn->updateHover(uiMousePos);
            _speed2Btn->updateHover(uiMousePos);
            _speed4Btn->updateHover(uiMousePos);
        }

        if (_isPresetListOpen && hasFocus) {
            sf::Vector2f listMousePos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _presetListView);
            for (auto& btn : _presetButtons) {
                btn->updateHover(listMousePos);
            }
        }
        if (_isStatusTimerActive && _statusTimer.getElapsedTime().asSeconds() > 3.0f) {
            _presetStatusLabel->setString("");
            _isStatusTimerActive = false;
        }
        
        if (hasFocus && sf::Mouse::getPosition(_mainWindow).x < 1000 && !_isPanning && !_isPresetListOpen) {
            if (_selectedPresetIndex != -1) {
                sf::Vector2f worldPos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _boardView);
                _board->setGhost(&_presets[_selectedPresetIndex], static_cast<int>(worldPos.x / 20), static_cast<int>(worldPos.y / 20));
            }
            else {
                _board->clearGhost();
                
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !_blockDrawing) {
                    sf::Vector2f worldPos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _boardView);
                    _board->setCellState(static_cast<int>(worldPos.x / 20), static_cast<int>(worldPos.y / 20), true);
                }
                else if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
                    sf::Vector2f worldPos = _mainWindow.mapPixelToCoords(sf::Mouse::getPosition(_mainWindow), _boardView);
                    _board->setCellState(static_cast<int>(worldPos.x / 20), static_cast<int>(worldPos.y / 20), false);
                }
            }
        }
        else {
            
            _board->clearGhost();
        }

        
        if (_isSimulationRunning && !_isPresetListOpen) {
            if (_simClock.getElapsedTime() >= _simInterval) {
                _board->updateBoard();
                _simClock.restart();
            }
        }
    }
}

void Root::render() {
    _mainWindow.clear(sf::Color(35, 35, 35));

    if (_currentState == AppState::Menu) {
        _mainWindow.setView(_uiView);
        _mainWindow.draw(_menuBgSprite);

        _menuTitle->draw(_mainWindow);
        _sizeInput->draw(_mainWindow);
        _confirmButton->draw(_mainWindow);
        _errorLabel->draw(_mainWindow);
    }
    else if (_currentState == AppState::Simulation) {
        _mainWindow.setView(_boardView);
        _board->draw(_mainWindow);

        _mainWindow.setView(_uiView);
        _mainWindow.draw(_uiPanelBackground);
        _uiTitleLabel->draw(_mainWindow);
        _authorLabel->draw(_mainWindow);
        _startStopButton->draw(_mainWindow);
        _clearButton->draw(_mainWindow);
        _legendLabel->draw(_mainWindow);
        _speed05Btn->draw(_mainWindow);
        _speed1Btn->draw(_mainWindow);
        _speed2Btn->draw(_mainWindow);
        _speed4Btn->draw(_mainWindow);

        _randomizeButton->draw(_mainWindow);
        _randomPercentLabel->draw(_mainWindow);
        _randomPercentInput->draw(_mainWindow);

        _wrapEdgesButton->draw(_mainWindow);

        _savePresetBtn->draw(_mainWindow);
        _presetNameInput->draw(_mainWindow);
        _presetStatusLabel->draw(_mainWindow);
        _loadPresetBtn->draw(_mainWindow);

        _backButton->draw(_mainWindow);

        if (_isPresetListOpen) {
            _mainWindow.setView(_uiView);
            _mainWindow.draw(_presetListBg);

            _mainWindow.setView(_presetListView);
            for (auto& btn : _presetButtons) {
                btn->draw(_mainWindow);
            }
        }
    }

    _mainWindow.display();
}

void Root::setActiveSpeed(int index) {
    sf::Color inactive(70, 70, 70);
    sf::Color active(70, 100, 130);

    _speed05Btn->setBaseColor(index == 0 ? active : inactive);
    _speed1Btn->setBaseColor(index == 1 ? active : inactive);
    _speed2Btn->setBaseColor(index == 2 ? active : inactive);
    _speed4Btn->setBaseColor(index == 3 ? active : inactive);
}