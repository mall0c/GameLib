#include "gamelib/Game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "gamelib/GameState.hpp"
#include "gamelib/utils/log.hpp"
#include "gamelib/event/SFMLEvent.hpp"

#define GAME_WIDTH 640
#define GAME_HEIGHT 480
#define GAME_TITLE "Unnamed game"
#define GAME_MAX_FPS 0
#define GAME_VSYNC false

namespace gamelib
{
    Game::Game()
    { }

    Game::~Game()
    {
        destroy();
    }


    bool Game::init()
    {
        LOG("Initializing game...");
        _window.create(sf::VideoMode(GAME_WIDTH, GAME_HEIGHT), GAME_TITLE, sf::Style::Close);
        _window.setFramerateLimit(GAME_MAX_FPS);
        _window.setVerticalSyncEnabled(GAME_VSYNC);

        return true;
    }

    void Game::run()
    {
        float fps = 1; // TODO: Consider switching to double
        sf::Clock clock;
        std::shared_ptr<SFMLEvent> ev(new SFMLEvent());

        while (_window.isOpen())
        {
            clock.restart();

            while (_window.pollEvent(ev->ev))
            {
                switch (ev->ev.type)
                {
                    case sf::Event::KeyPressed:
                        if (ev->ev.key.code == sf::Keyboard::Escape || ev->ev.key.code == sf::Keyboard::Q)
                        {
                            close();
                            return;
                        }
                        break;

                    case sf::Event::Closed:
                        close();
                        return;

                    case sf::Event::GainedFocus:
                        _active = true;
                        break;

                    case sf::Event::LostFocus:
                        _active = false;
                        break;
                }
                _evmgr.triggerEvent(ev);
                ev.reset(new SFMLEvent());
            }

            for (auto& i : _states)
            {
                i->update(fps);
            }

            _evmgr.update();

            _window.clear(_bgcolor);
            for (auto& i : _states)
            {
                i->render(_window);
            }
            _window.display();

            //calculate fps
            sf::Int32 elapsed = clock.getElapsedTime().asMilliseconds();
            fps = 1000.0f / (elapsed ? elapsed : 1);
            LOG_DEBUG_RAW("\r", LOG_DUMP(elapsed), "ms, ", LOG_DUMP(fps), "     \r"); // write some spaces to overwrite existing characters
        }
    }

    void Game::close()
    {
        if (_window.isOpen())
        {
            _window.close();
            LOG_DEBUG("Game window closed");
        }
    }

    void Game::destroy()
    {
        close();
        if (!_states.empty())
        {
            LOG_DEBUG("Closing game states...");
            for (auto& i : _states)
            {
                i->quit();
            }
            _states.clear();
        }
    }


    bool Game::loadFromJson(const Json::Value& node)
    {
        if (node.isMember("title"))
        {
            LOG_DEBUG("title: ", node["title"].asString());
            _window.setTitle(node["title"].asString());
        }

        auto size = _window.getSize();
        size.x = node.get("width", size.x).asUInt();
        size.y = node.get("height", size.y).asUInt();
        LOG_DEBUG(LOG_DUMP(size.x), ", ", LOG_DUMP(size.y));
        _window.setSize(size);

        if (node.isMember("maxfps"))
        {
            LOG_DEBUG("maxfps: ", node["maxfps"].asUInt());
            _window.setFramerateLimit(node["maxfps"].asUInt());
        }

        if (node.isMember("vsync"))
        {
            LOG_DEBUG("vsync: ", node["vsync"].asBool());
            _window.setVerticalSyncEnabled(node["vsync"].asBool());
        }

        if (node.isMember("bg"))
        {
            const auto& bg = node["bg"];
            _bgcolor.r = bg.get("r", _bgcolor.r).asUInt();
            _bgcolor.g = bg.get("g", _bgcolor.g).asUInt();
            _bgcolor.b = bg.get("b", _bgcolor.b).asUInt();
        }

        return true;
    }


    void Game::pushState(std::unique_ptr<GameState> state)
    {
        if (!state->init(this))
        {
            LOG_ERROR("Failed to initialize game state");
            return;
        }
        _states.push_back(std::move(state));
        LOG_DEBUG("Game state added");
    }

    void Game::popState()
    {
        _states.back()->quit();
        _states.pop_back();
    }

    GameState& Game::pullState() const 
    {
        return *_states.back().get();
    }

    bool Game::isActive() const
    {
        return _active;
    }

    bool Game::isKeyPressed(sf::Keyboard::Key key) const
    {
        return _active && sf::Keyboard::isKeyPressed(key);
    }

    void Game::regEventCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        _evmgr.regCallback(id, callback, me);
        LOG_DEBUG("Added event callback: ev: ", id, ", obj: ", me);
    }

    void Game::unregEventCallback(EventID id, void (*callback)(void*, EventPtr), void* me)
    {
        _evmgr.unregCallback(id, callback, me);
        LOG_DEBUG("Removed event callback: ev: ", id, ", obj: ", me);
    }
}

