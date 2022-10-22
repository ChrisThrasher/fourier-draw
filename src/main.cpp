#include "Epicycles.hpp"
#include "Line.hpp"

#include <cmath>

using namespace sf::Literals;

static constexpr auto width = 1280u;
static constexpr auto height = 720u;

static auto transform(const Line& line, Epicycles& x_epicycles, Epicycles& y_epicycles)
{
    auto x_signal = std::vector<float>(line.size());
    auto y_signal = std::vector<float>(line.size());
    for (size_t i = 0; i < line.size(); ++i) {
        const auto point = line.at(i);
        x_signal[i] = point.x - width / 2.f;
        y_signal[i] = point.y - height / 2.f;
    }

    x_epicycles = { discrete_fourier_transform(x_signal), { width / 2.f, 200 }, 0_deg };
    y_epicycles = { discrete_fourier_transform(y_signal), { 200, height / 2.f }, 90_deg };
}

int main()
{
    auto x_epicycles = Epicycles();
    auto y_epicycles = Epicycles();
    auto signal = Line();
    auto line = Line();
    auto line_shadow = Line();
    auto frame_count = size_t(0);

    auto font = sf::Font();
    if (!font.loadFromFile(FONT_PATH / std::filesystem::path("font.ttf")))
        throw std::runtime_error("Failed to load font");

    auto text = sf::Text("Click and drag to draw a curve", font, 48);
    text.setOrigin({ text.getLocalBounds().left + text.getLocalBounds().width / 2,
                     text.getLocalBounds().top + text.getLocalBounds().height / 2 });
    text.setPosition({ width / 2.f, 150 });
    text.setFillColor(sf::Color::White);

    auto window = sf::RenderWindow(sf::VideoMode({ width, height }), "Fourier Draw");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        for (auto event = sf::Event(); window.pollEvent(event);) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    signal = {};
                    x_epicycles = {};
                    y_epicycles = {};
                    break;
                default:
                    break;
                }
            default:
                break;
            }
        }

        window.clear();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            const auto mouse = sf::Mouse::getPosition(window);
            const auto is_within_x = mouse.x >= 0 && mouse.x <= int(window.getSize().x);
            const auto is_within_y = mouse.y >= 0 && mouse.y <= int(window.getSize().y);
            if (is_within_x && is_within_y) {
                const auto position = sf::Vector2f(float(mouse.x) * width / float(window.getSize().x),
                                                   float(mouse.y) * height / float(window.getSize().y));
                const auto reset_line
                    = [position, &signal, &line, &line_shadow, &x_epicycles, &y_epicycles, &frame_count]() {
                          signal.push_back(position);
                          line = {};
                          line_shadow = signal;
                          transform(signal, x_epicycles, y_epicycles);
                          frame_count = 0;
                      };
                if (signal.empty()) {
                    reset_line();
                } else {
                    const auto vector = position - signal.back();
                    if (vector.length() > 2)
                        reset_line();
                }
            }
        }

        if (x_epicycles.empty()) {
            window.draw(text);
            window.display();
            continue;
        }

        line_shadow.set_brightness(uint8_t(std::max(196 - 4 * int(frame_count), 0)));
        window.draw(line_shadow);

        if (frame_count % 2 == 0) {
            x_epicycles.update();
            y_epicycles.update();
            line.push_back(signal.at(frame_count / 2));
        }

        window.draw(x_epicycles);
        window.draw(y_epicycles);
        window.draw(line);

        auto vertical = sf::RectangleShape({ stroke, y_epicycles.tip().y - x_epicycles.tip().y });
        vertical.setOrigin({ 0, stroke / 2 });
        vertical.setPosition(x_epicycles.tip());
        vertical.setFillColor(color);
        window.draw(vertical);

        auto horizontal = sf::RectangleShape({ x_epicycles.tip().x - y_epicycles.tip().x, stroke });
        horizontal.setOrigin({ 0, stroke / 2 });
        horizontal.setPosition(y_epicycles.tip());
        horizontal.setFillColor(color);
        window.draw(horizontal);

        if (++frame_count == 2 * x_epicycles.size()) {
            frame_count = 0;
            line_shadow = line;
            line = {};
        }

        window.display();
    }
}
