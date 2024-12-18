#include "Epicycles.hpp"
#include "Line.hpp"

using namespace sf::Literals;

namespace {
constexpr auto width = 1280u;
constexpr auto height = 720u;

auto transform(const Line& line, Epicycles& x_epicycles, Epicycles& y_epicycles)
{
    auto x_signal = std::vector<float>(line.size());
    auto y_signal = std::vector<float>(line.size());
    for (std::size_t i = 0; i < line.size(); ++i) {
        const auto point = line.at(i);
        x_signal[i] = point.x - width / 2.f;
        y_signal[i] = point.y - height / 2.f;
    }

    x_epicycles = { discrete_fourier_transform(x_signal), { width / 2.f, 200 }, 0_deg };
    y_epicycles = { discrete_fourier_transform(y_signal), { 200, height / 2.f }, 90_deg };
}
}

int main()
{
    auto x_epicycles = Epicycles();
    auto y_epicycles = Epicycles();
    auto signal = Line();
    auto line = Line();
    auto line_shadow = Line();
    auto frame_count = std::size_t(0);

    const auto font = sf::Font(FONT_PATH / std::filesystem::path("font.ttf"));

    auto text = sf::Text(font, "Click and drag to draw a curve", 48);
    text.setOrigin(text.getLocalBounds().getCenter());
    text.setPosition({ width / 2.f, 150 });
    text.setFillColor(sf::Color::White);

    auto window = sf::RenderWindow(sf::VideoMode({ width, height }), "Fourier Draw");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key_pressed = event->getIf<sf::Event::KeyPressed>()) {
                switch (key_pressed->code) {
                case sf::Keyboard::Key::Escape:
                    window.close();
                    break;
                case sf::Keyboard::Key::Space:
                    signal = {};
                    x_epicycles = {};
                    y_epicycles = {};
                    break;
                default:
                    break;
                }
            }
        }

        window.clear();

        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
            const auto mouse = sf::Mouse::getPosition(window);
            const auto is_within_x = mouse.x >= 0 && mouse.x <= int(window.getSize().x);
            const auto is_within_y = mouse.y >= 0 && mouse.y <= int(window.getSize().y);
            if (is_within_x && is_within_y) {
                static constexpr auto minimum_distance_between_points = 5;
                const auto position = sf::Vector2f(float(mouse.x) * width / float(window.getSize().x),
                                                   float(mouse.y) * height / float(window.getSize().y));
                if (signal.empty() || (position - signal.back()).length() > minimum_distance_between_points) {
                    signal.push_back(position);
                    line = {};
                    line_shadow = signal;
                    transform(signal, x_epicycles, y_epicycles);
                    frame_count = 0;
                };
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
