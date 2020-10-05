#include "Widgets.h"

std::unique_ptr<Colors::Color> Widgets::current_color (std::make_unique<Colors::Red>());

const std::set<Widgets::COLORS> data_base_color {
        Widgets::COLORS::RED,
        Widgets::COLORS::GREEN,
        Widgets::COLORS::BLUE
};

int Widgets::button_type = 0;

Widgets::CreateModelButton_Rectangle::CreateModelButton_Rectangle(std::vector<std::unique_ptr<Primitives::IShape>> & global_ground, float x, float y) : ground(global_ground){
    auto ColorButton = Colors::White();
    ColorButton.Transparency(0.5f);
    auto Color = ColorButton.GetColor();

    std::vector<float> verteces {
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    this->sprite = std::make_unique<Primitives::Rectangle>(verteces);

    *this->sprite.value()->GetPosition().first = x;
    *this->sprite.value()->GetPosition().second = y;
}

void Widgets::CreateModelButton_Rectangle::Draw() {
    this->sprite.value()->Draw();
}

bool Widgets::CreateModelButton_Rectangle::Button(float x_mouse, float y_mouse) {
    return this->sprite.value()->Crossing(x_mouse, y_mouse);
}

void Widgets::CreateModelButton_Rectangle::Action() {
    Widgets::button_type = 0;
    auto Color = current_color->GetColor();

    std::vector<float> verteces {
            -0.2f, -0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.2f, -0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.2f,  0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.2f,  0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.2f,  0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.2f, -0.2f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    ground.emplace_back(std::make_unique<Primitives::Rectangle>(verteces));
}

Widgets::CreateModelButton_Triangle::CreateModelButton_Triangle(std::vector<std::unique_ptr<Primitives::IShape>> & global_ground, float x, float y) : ground(global_ground) {
    auto ColorButton = Colors::White();
    ColorButton.Transparency(0.5f);
    auto Color = ColorButton.GetColor();

    std::vector<float> verteces {
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.0f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    this->sprite = std::make_unique<Primitives::Rectangle>(verteces);

    *this->sprite.value()->GetPosition().first = x;
    *this->sprite.value()->GetPosition().second = y;
}

void Widgets::CreateModelButton_Triangle::Draw() {
    this->sprite.value()->Draw();
}

bool Widgets::CreateModelButton_Triangle::Button(float x_mouse, float y_mouse) {
    return this->sprite.value()->Crossing(x_mouse, y_mouse);
}

void Widgets::CreateModelButton_Triangle::Action() {
    Widgets::button_type = 1;
    auto Color = current_color->GetColor();

    std::vector<float> verteces {
            -0.2f, -0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.2f, -0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.0f,  0.2f, 0.f, Color[0], Color[1], Color[2], Color[3],
    };

    ground.emplace_back(std::make_unique<Primitives::Rectangle>(verteces));
}


Widgets::CreateColorButton::CreateColorButton(float x, float y, COLORS color) {

    this->color = color;
    std::vector<float> Color;

    switch (color) {
        case Widgets::COLORS::RED :
            Color = Colors::Red().GetColor();
            break;
        case Widgets::COLORS::GREEN :
            Color = Colors::Green().GetColor();
            break;
        case Widgets::COLORS::BLUE :
            Color = Colors::Blue().GetColor();
            break;
    }

    std::vector<float> verteces_trian {
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.0f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    this-> sprite_trian = std::make_unique<Primitives::Rectangle>(verteces_trian);

    *this->sprite_trian.value()->GetPosition().first = x;
    *this->sprite_trian.value()->GetPosition().second = y;

    std::vector<float> verteces_rect {
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.13f,  0.13f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.13f, -0.13f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    this->sprite_rect = std::make_unique<Primitives::Rectangle>(verteces_rect);

    *this->sprite_rect.value()->GetPosition().first = x;
    *this->sprite_rect.value()->GetPosition().second = y;

}

void Widgets::CreateColorButton::Draw() {
    switch(::Widgets::button_type) {

        case 0 :
            sprite_rect.value()->Draw();
            break;

        case 1 :
            sprite_trian.value()->Draw();
            break;

        default:
            sprite_rect.value()->Draw();
            break;

    }
}

bool Widgets::CreateColorButton::Button(float x_mouse, float y_mouse) {
    switch(::Widgets::button_type) {

        case 0 :
            return sprite_rect.value()->Crossing(x_mouse, y_mouse);
            break;

        case 1 :
            return sprite_trian.value()->Crossing(x_mouse, y_mouse);
            break;

        default:
            return sprite_rect.value()->Crossing(x_mouse, y_mouse);
            break;

    }
}

void Widgets::CreateColorButton::Action() {
    current_color.release();
    switch (color) {
        case Widgets::COLORS::RED :
            current_color = std::make_unique<Colors::Red>(Colors::Red());
            break;
        case Widgets::COLORS::GREEN :
            current_color = std::make_unique<Colors::Green>(Colors::Green());
            break;
        case Widgets::COLORS::BLUE :
            current_color = std::make_unique<Colors::Blue>(Colors::Blue());
            break;
    }
}

Widgets::NewGroundButton::NewGroundButton(std::vector<std::unique_ptr<Primitives::IShape>> & global_ground, float x, float y) : ground(global_ground) {
    auto ColorButton = Colors::White();
    ColorButton.Transparency(0.5f);
    auto Color = ColorButton.GetColor();

    std::vector<float> verteces {
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };

    this->sprite = std::make_unique<Primitives::Rectangle>(verteces);

    *this->sprite.value()->GetPosition().first = x;
    *this->sprite.value()->GetPosition().second = y;
}

void Widgets::NewGroundButton::Draw() {
    this->sprite.value()->Draw();
}

bool Widgets::NewGroundButton::Button(float x_mouse, float y_mouse) {
    return this->sprite.value()->Crossing(x_mouse, y_mouse);
}

void Widgets::NewGroundButton::Action() {
    this->ground.clear();
}

Widgets::ImportGroundButton::ImportGroundButton(std::vector<std::unique_ptr<Primitives::IShape>> & global_ground, float x, float y) : ground(global_ground) {
    auto ColorButton = Colors::Blue();
    ColorButton.Transparency(0.5f);
    auto Color = ColorButton.GetColor();

    std::vector<float> verteces {
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };


    this->sprite = std::make_unique<Primitives::Rectangle>(verteces);

    *this->sprite.value()->GetPosition().first = x;
    *this->sprite.value()->GetPosition().second = y;
}

void Widgets::ImportGroundButton::Draw() {
    this->sprite.value()->Draw();
}

bool Widgets::ImportGroundButton::Button(float x_mouse, float y_mouse) {
    return this->sprite.value()->Crossing(x_mouse, y_mouse);
}

float LoadNumber(std::fstream& input) {
    double result = 0;
    double sign = 1;
    if (input.peek() == '-')
    {
        char ch;
        input >> ch;
        sign = -1;
    }
    while (isdigit(input.peek())) {
        result *= 10;
        result += input.get() - '0';
    }
    if (input.peek() == '.')
    {
        input.get();
        double part = 0.1;
        while (isdigit(input.peek())) {
            result += (input.get() - '0') * part;
            part *= 0.1;
        }
        return float(result * sign);
    } else
        return float(result * sign);
}

std::vector<std::unique_ptr<Primitives::IShape>> Widgets::DeSerialize(std::fstream & stream){
    std::vector<std::unique_ptr<Primitives::IShape>> tmp;
    char trash;
    std::string type;
    std::vector<float> vertices;
    float x, y;
    while (stream >> trash && trash == '['){
        std::getline(stream, type);
        if (type == "RECTANGLE]"){
            for (size_t i = 0; i < 42; i++){
                vertices.emplace_back(LoadNumber(stream));
                stream >> trash;
            }
            tmp.emplace_back(std::make_unique<Primitives::Rectangle>(vertices));
        }
        else if (type == "TRIANGLE]"){
            for (size_t i = 0; i < 21; i++){
                vertices.emplace_back(LoadNumber(stream));
                stream >> trash;
            }
            tmp.emplace_back(std::make_unique<Primitives::Triangle>(vertices));
        }
        x = LoadNumber(stream);
        stream >> trash;
        y = LoadNumber(stream);
        *tmp.back()->GetPosition().first = x;
        *tmp.back()->GetPosition().second = y;
        vertices.clear();
    }
    return std::move(tmp);
}


void Widgets::ImportGroundButton::Action() {
    std::cout << "Write in console filename to load: " << std::endl;
    std::string filename;
    std::cin >> filename;
    std::fstream file (std::filesystem::current_path().parent_path().parent_path()/filename);
    if (!file.is_open()){
        std::cout << "File on path " << std::filesystem::current_path().parent_path().parent_path()/filename.c_str() << " not found!" << std::endl;
        return;
    }
    ground.clear();
    ground = Widgets::DeSerialize(file);

    file.close();
}

Widgets::ExportGroundButton::ExportGroundButton(std::vector<std::unique_ptr<Primitives::IShape>> & global_ground, float x, float y)
                                                 : ground(global_ground) {
    auto ColorButton = Colors::Green();
    ColorButton.Transparency(0.5f);
    auto Color = ColorButton.GetColor();

    std::vector<float> verteces {
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f,  0.06f, 0.f, Color[0], Color[1], Color[2], Color[3],
            -0.15f, -0.06f, 0.f, Color[0], Color[1], Color[2], Color[3]
    };


    this->sprite = std::make_unique<Primitives::Rectangle>(verteces);

    *this->sprite.value()->GetPosition().first = x;
    *this->sprite.value()->GetPosition().second = y;
}

void Widgets::ExportGroundButton::Draw() {
    this->sprite.value()->Draw();
}

bool Widgets::ExportGroundButton::Button(float x_mouse, float y_mouse) {
    return this->sprite.value()->Crossing(x_mouse, y_mouse);
}


std::string Widgets::Serialize(std::vector<std::unique_ptr<Primitives::IShape>>& data){
    std::string to_export;
    std::pair<float *, float *> xy_pos;
    for (auto & i : data){
        if (i->GetVertices().size() == 42){
            to_export += "[RECTANGLE]\n";
            for (auto & j : i->GetVertices()){
                to_export += std::to_string(j) + ',';
            }
        } else if (i->GetVertices().size() == 21){
            to_export += "[TRIANGLE]\n";
            for (auto & j : i->GetVertices()){
                to_export += std::to_string(j) + ',';
            }
        }
        xy_pos = i->GetPosition();
        to_export += std::to_string(*xy_pos.first) + "," + std::to_string(*xy_pos.second);
        to_export += '\n';
    }

    return std::move(to_export);
}

void Widgets::ExportGroundButton::Action() {
    std::cout << "Write in console filename to save: " << std::endl;
    std::string filename;
    std::cin >> filename;
    std::ofstream file (std::filesystem::current_path().parent_path().parent_path()/filename);

    file << Serialize(ground);

    file.close();
}

Widgets::BuildWidgets &Widgets::BuildWidgets::Instance() {
    static BuildWidgets bw;
    return bw;
}

void Widgets::BuildWidgets::SetButtonRectangle(std::vector<std::unique_ptr<Primitives::IShape>> & vec, float x, float y) {
    Widgets.emplace_back(std::make_unique<CreateModelButton_Rectangle>(vec, x, y));
}

void Widgets::BuildWidgets::SetButtonTriangle(std::vector<std::unique_ptr<Primitives::IShape>> & vec, float x, float y) {
    Widgets.emplace_back(std::make_unique<CreateModelButton_Triangle>(vec, x, y));
}

void Widgets::BuildWidgets::SetButtonColor(float x, float y, Widgets::COLORS color) {
    Widgets.emplace_back(std::make_unique<CreateColorButton>(x, y, color));
}

void Widgets::BuildWidgets::SetImportButton(std::vector<std::unique_ptr<Primitives::IShape>> & vec, float x, float y) {
    Widgets.emplace_back(std::make_unique<ImportGroundButton>(vec, x, y));
}

void Widgets::BuildWidgets::SetExportButton(std::vector<std::unique_ptr<Primitives::IShape>> & vec, float x, float y) {
    Widgets.emplace_back(std::make_unique<ExportGroundButton>(vec, x, y));
}

void Widgets::BuildWidgets::SetNewButton(std::vector<std::unique_ptr<Primitives::IShape>> & vec, float x, float y) {
    Widgets.emplace_back(std::make_unique<NewGroundButton>(vec, x, y));
}
