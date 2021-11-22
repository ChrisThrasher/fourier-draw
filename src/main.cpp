#include "dft.h"
#include "epicycle.h"
#include "line.h"

#include <algorithm>
#include <cmath>

static constexpr auto width = 1920u;
static constexpr auto height = 1080u;

struct EpicycleSet {
    std::vector<Epicycle> x;
    std::vector<Epicycle> y;
};

static auto Transform(const Line& line)
{
    struct {
        std::vector<float> x;
        std::vector<float> y;
    } signal;
    for (size_t i = 0; i < line.Size(); ++i) {
        const auto point = line.At(i);
        signal.x.push_back(point.x - width / 2.0f);
        signal.y.push_back(point.y - height / 2.0f);
    }

    auto epicycle_set = EpicycleSet();
    for (const auto& dft_datum : DiscreteFourierTransform(signal.x))
        epicycle_set.x.emplace_back(dft_datum.amplitude, dft_datum.frequency, dft_datum.phase);
    std::stable_sort(epicycle_set.x.begin(), epicycle_set.x.end());
    epicycle_set.x.front().SetPosition({ width / 2.0f, 200.0f });

    for (const auto& dft_datum : DiscreteFourierTransform(signal.y))
        epicycle_set.y.emplace_back(dft_datum.amplitude, dft_datum.frequency, dft_datum.phase + half_pi);
    std::stable_sort(epicycle_set.y.begin(), epicycle_set.y.end());
    epicycle_set.y.front().SetPosition({ 200.0f, height / 2.0f });

    return epicycle_set;
}

int main()
{
    auto epicycle_set = EpicycleSet();
    auto signal = Line();
    auto line = Line();
    auto line_shadow = Line();
    auto frame_count = (size_t)0;

    auto font = sf::Font();
    font.loadFromFile(std::string(FONT_PATH) + "/font.ttf");

    auto text = sf::Text("Click and drag to draw a curve", font, 48);
    text.setOrigin({ text.getLocalBounds().left + text.getLocalBounds().width / 2.0f,
                     text.getLocalBounds().top + text.getLocalBounds().height / 2.0f });
    text.setPosition({ width / 2.0f, 150.0f });
    text.setFillColor(sf::Color::White);

    auto window = sf::RenderWindow(sf::VideoMode(width, height), "Fourier Draw");
    window.setFramerateLimit(60);
    while (window.isOpen()) {
        auto event = sf::Event();
        while (window.pollEvent(event)) {
            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                switch (event.key.code) {
                case sf::Keyboard::Space:
                    signal = {};
                    epicycle_set = {};
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
            if (mouse.x >= 0 && mouse.x <= (int)width && mouse.y >= 0 && mouse.y <= (int)height) {
                const auto position = sf::Vector2f((float)mouse.x, (float)mouse.y);
                if (signal.Empty()) {
                    signal.PushBack(position);
                    line = {};
                    line_shadow = signal;
                    epicycle_set = Transform(signal);
                    frame_count = 0;
                } else {
                    const auto vector = position - signal.Back();
                    const auto length = std::sqrt(vector.x * vector.x + vector.y * vector.y);
                    if (length > 2.0f) {
                        signal.PushBack(position);
                        line = {};
                        line_shadow = signal;
                        epicycle_set = Transform(signal);
                        frame_count = 0;
                    }
                }
            }
        }

        if (epicycle_set.x.empty()) {
            window.draw(text);
            window.display();
            continue;
        }

        line_shadow.SetBrightness((sf::Uint8)std::max(196 - 4 * (int)frame_count, 0));
        line_shadow.Draw(window);

        const auto update = [&window](std::vector<Epicycle>& epicycles) {
            const auto dt = 2.0f * pi / (float)epicycles.size();
            for (size_t i = 0; i < epicycles.size(); ++i) {
                if (i == 0)
                    epicycles[i].Update(dt);
                else
                    epicycles[i].Update(dt, epicycles[i - 1].GetPosition());
                epicycles[i].Draw(window);
            }
        };
        update(epicycle_set.x);
        update(epicycle_set.y);

        line.PushBack(signal.At(frame_count));
        line.Draw(window);

        constexpr auto stroke = 2.5f;
        auto vertical = sf::RectangleShape(
            { stroke, epicycle_set.y.back().GetPosition().y - epicycle_set.x.back().GetPosition().y });
        vertical.setOrigin({ 0.0f, stroke / 2.0f });
        vertical.setPosition(epicycle_set.x.back().GetPosition());
        vertical.setFillColor(sf::Color(255, 255, 255, 128));
        window.draw(vertical);

        auto horizontal = sf::RectangleShape(
            { epicycle_set.x.back().GetPosition().x - epicycle_set.y.back().GetPosition().x, stroke });
        horizontal.setOrigin({ 0.0f, stroke / 2.0f });
        horizontal.setPosition(epicycle_set.y.back().GetPosition());
        horizontal.setFillColor(sf::Color(255, 255, 255, 128));
        window.draw(horizontal);

        if (++frame_count == epicycle_set.x.size()) {
            frame_count = 0;
            line_shadow = line;
            line = {};
        }

        window.display();
    }
}
