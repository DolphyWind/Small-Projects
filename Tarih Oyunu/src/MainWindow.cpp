#include "MainWindow.hpp"

MainWindow::~MainWindow()
{

}

void MainWindow::mainLoop() 
{
    while(window.isOpen())
    {
        handleEvents();
        update();
        draw();
    }
}

MainWindow::MainWindow(sf::Vector2u size, sf::String title, sf::Uint32 style) 
{
    std::srand(std::time(NULL));
    this->windowSize = size;
    this->windowTitle = title;
    this->windowStyle = style;
    this->window.create(sf::VideoMode(this->windowSize.x, this->windowSize.y), this->windowTitle, style);
    this->window.setFramerateLimit(60);
    #ifdef _WIN32
    homedir = getenv("appdata");
    homedirStr = homedir;
    if(!homedirStr.empty())
    {
        homedirStr += '\\';
        saveFolderPath = homedirStr += "AltmisSaniyedeTarih\\";
        try
        {
            if(!std::filesystem::is_directory(saveFolderPath)) std::filesystem::create_directory(saveFolderPath);
        }
        catch(const std::exception& e) {}

        filePath = saveFolderPath + "game.data";
        std::ifstream ifs(filePath, std::ios::binary);
        bool opened = ifs.good();
        if(opened)
            ifs.read((char*)&highScore, sizeof(std::int64_t));
        ifs.close();
        if(!opened) saveHighscore();
    }
    #else
    if((homedir = getenv("HOME")) == NULL)
        homedir = getpwuid(getuid())->pw_dir;
    homedirStr = homedir;
    if(!homedirStr.empty())
    {
        homedirStr += '/';
        saveFolderPath = homedirStr + ".AltmisSaniyedeTarih/";
        try
        {
            if(!std::filesystem::is_directory(saveFolderPath)) std::filesystem::create_directory(saveFolderPath);
        }
        catch(const std::exception& e) {}
        filePath = saveFolderPath + "game.data";
        std::ifstream ifs(filePath, std::ios::binary);
        bool opened = ifs.good();
        if(opened)
            ifs.read((char*)&highScore, sizeof(std::int64_t));
        ifs.close();
        if(!opened) saveHighscore();
    }
    #endif

    createQuestions();
    loadAssets();
    createMainMenu();
    createIngame();
    createEndScreen();
}

void MainWindow::handleEvents() 
{
    while (window.pollEvent(e))
    {
        if(e.type == sf::Event::Closed) window.close();
    }
}

void MainWindow::update() 
{
    if(!sf::Mouse::isButtonPressed(sf::Mouse::Left)) lPress = true;
    switch (this->currentScene)
    {
    case Scene::MainMenu:
        updateMainMenu();
        break;
    case Scene::Ingame:
        updateIngame();
        break;
    case Scene::EndScreen:
        updateEndScreen();
        break;
    }
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) lPress = false;
}

void MainWindow::draw() 
{
    window.clear(backgroundColor);
    
    switch (this->currentScene)
    {
    case Scene::MainMenu:
        drawMainMenu();
        break;
    case Scene::Ingame:
        drawIngame();
        break;
    case Scene::EndScreen:
        drawEndScreen();
        break;
    }

    window.display();
}

void MainWindow::createAnswerButton(Button &btn, sf::String str) 
{
    btn = Button(&window, &font, sf::Vector2f(266, 32), 14, str);
    btn.setNormalTexture(&buttonTexture);

    btn.setEventMode(EventMode::COLOR_WITH_TEXTURE);
    btn.setTextAlignment(TextAlignment::LEFT);
    btn.setNormalColor(sf::Color::White);
    btn.setHoverColor(sf::Color(225, 225, 225));
    btn.setPressedColor(sf::Color(100, 100, 100));
    btn.setTextColor(sf::Color::White);
    btn.setTextOffset(15, -2);
}

void MainWindow::createMainMenuButtons(Button &btn, sf::String str) 
{
    btn = Button(&window, &font, sf::Vector2f(200, 60), 18, str);
    btn.setEventMode(EventMode::COLOR);
    btn.setTextAlignment(TextAlignment::CENTER);
    btn.setTextOffset(0, -2);
    btn.setOutlineThickness(2);
    btn.setOutlineColor(sf::Color::Black);
}

void MainWindow::createTimerShape() 
{
    this->timerShape.setPointCount(361);
    this->outerTimer.setRadius(TIMER_RADIUS);
    this->outerTimer.setOutlineThickness(3);
    sf::Vector2f outerTimerPos = sf::Vector2f(1, 1) * this->outerTimer.getOutlineThickness();
    outerTimerPos.x = this->windowSize.x - outerTimerPos.x - 2 * TIMER_RADIUS;
    this->outerTimer.setOutlineColor(sf::Color::Black);
    this->outerTimer.setPosition(outerTimerPos);
    this->outerTimer.setFillColor(sf::Color::Transparent);
    this->timerShape.setFillColor(sf::Color::Red);
    this->timerShape.setPosition(outerTimerPos + sf::Vector2f(TIMER_RADIUS, TIMER_RADIUS));
}

void MainWindow::createTimerText() 
{
    this->timerText.setFont(font);
    this->timerText.setCharacterSize(18);
}

void MainWindow::calcPosTimerText() 
{
    sf::Vector2f outerTimerPos = this->outerTimer.getPosition();
    sf::Vector2f timerTextSize;
    timerTextSize.x = this->timerText.getLocalBounds().width + this->timerText.getLocalBounds().left;
    timerTextSize.y = this->timerText.getLocalBounds().height + this->timerText.getLocalBounds().top;

    this->timerText.setPosition(
        outerTimerPos.x + TIMER_RADIUS - (timerTextSize.x / 2),
        outerTimerPos.y + TIMER_RADIUS - (timerTextSize.y / 2) - 2
    );
}

void MainWindow::createUIButtons() 
{
    createAnswerButton(this->questionField, "Question goes here...");
    this->questionField.setSize(sf::Vector2f(626, 64));
    this->questionField.setNormalTexture(&this->questionFieldTexture);
    this->questionField.setDisabledColor(sf::Color::White);
    this->questionField.setDisabled(true);
    this->questionField.setPosition(0, 224);
    this->questionField.setTextAlignment(TextAlignment::TOPLEFT);
    this->questionField.setTextOffset(sf::Vector2f(80, 10));

    createAnswerButton(this->optionA, "A) ");
    createAnswerButton(this->optionB, "B) ");
    createAnswerButton(this->optionC, "C) ");
    createAnswerButton(this->optionD, "D) ");

    sf::Vector2f optAPos(24, this->questionField.getPosition().y + 96);
    this->optionA.setPosition(optAPos.x, optAPos.y);
    this->optionB.setPosition(optAPos.x + 313, optAPos.y);
    this->optionC.setPosition(optAPos.x, optAPos.y + 56);
    this->optionD.setPosition(optAPos.x + 313, optAPos.y + 56);
}

void MainWindow::loadAssets() 
{
    this->font.loadFromMemory(font_data, sizeof(font_data));
    this->buttonTexture.loadFromMemory(button_data, sizeof(button_data));
    this->questionFieldTexture.loadFromMemory(question_field_data, sizeof(question_field_data));
}

void MainWindow::updateTimerShape() 
{
    float elapsed = this->timerClock.getElapsedTime().asSeconds();
    if(elapsed >= maxTime)
    {
        this->currentScene = Scene::EndScreen;
        createEndText();
        isGrading = false;
        isBackwards = false;
        distanceGraded = 0;
        backgroundColor = originalBackgroundColor;
        return;
    }
    float measured = maxTime + 1 - elapsed;
    int measuredAsInt = measured;
    if(measuredAsInt != measuredAsIntPrev)
        this->timerAnim = !this->timerAnim;
    this->measuredAsIntPrev = measuredAsInt;
    float percentage = measured - measuredAsInt;
    int limitIndex = 360 * percentage;

    sf::Vector2f outerTimerPos = sf::Vector2f(1, 1) * this->outerTimer.getOutlineThickness();
    for(int i = 0; i < limitIndex; i++)
    {
        float deg = float(i) * M_PI / 180.f;
        if(!this->timerAnim)
            this->timerShape.setPoint(i, (float)TIMER_RADIUS * sf::Vector2f(-std::sin(deg), -std::cos(deg)));
        else
            this->timerShape.setPoint(i, sf::Vector2f(0, 0));
    }
    for(int i = limitIndex; i < 361; i++)
    {
        float deg = float(i) * M_PI / 180.f;
        if(!this->timerAnim)
            this->timerShape.setPoint(i, sf::Vector2f(0, 0));
        else
            this->timerShape.setPoint(i, (float)TIMER_RADIUS * sf::Vector2f(-std::sin(deg), -std::cos(deg)));
    }

    this->timerText.setString(std::to_string(measuredAsInt));
    calcPosTimerText();
}

void MainWindow::createIngame() 
{
    createUIButtons();
    createTimerShape();
    createTimerText();
    calcPosTimerText();
    createScoreText();
    selectQuestion(generateRandomIndex());
}

void MainWindow::updateIngame() 
{
    this->questionField.update();
    this->optionA.update();
    this->optionB.update();
    this->optionC.update();
    this->optionD.update();
    calcPosTimerText();
    updateTimerShape();
    gradeFunction();

    /*if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && lrPress)
    {
        currentIndex ++;
        if(currentIndex >= this->questions.size()) currentIndex = 0;
        this->selectQuestion(currentIndex);
    }
    else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && lrPress)
    {
        currentIndex --;
        if(currentIndex < 0) currentIndex = this->questions.size() - 1;
        this->selectQuestion(currentIndex);
    }
    lrPress = !(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right));
    */
    if(this->optionA.isClicked() && lPress)
    {
        if(this->questions[currentIndex]->getRightAnswer() == Answer::A) pickedRight();
        else pickedWrong();
        currentIndex = generateRandomIndex();
        this->selectQuestion(currentIndex);
    }
    if(this->optionB.isClicked() && lPress)
    {
        if(this->questions[currentIndex]->getRightAnswer() == Answer::B) pickedRight();
        else pickedWrong();
        currentIndex = generateRandomIndex();
        this->selectQuestion(currentIndex);
    }
    if(this->optionC.isClicked() && lPress)
    {
        if(this->questions[currentIndex]->getRightAnswer() == Answer::C) pickedRight();
        else pickedWrong();
        currentIndex = generateRandomIndex();
        this->selectQuestion(currentIndex);
    }
    if(this->optionD.isClicked() && lPress)
    {
        if(this->questions[currentIndex]->getRightAnswer() == Answer::D) pickedRight();
        else pickedWrong();
        currentIndex = generateRandomIndex();
        this->selectQuestion(currentIndex);
    }
}

void MainWindow::drawIngame() 
{
    this->questionField.draw();
    this->optionA.draw();
    this->optionB.draw();
    this->optionC.draw();
    this->optionD.draw();
    window.draw(this->outerTimer);
    window.draw(this->timerShape);
    window.draw(this->timerText);
    window.draw(scoreText);
}

void MainWindow::createMainMenu() 
{
    createMainMenuButtons(playButton, "Oyna");
    createMainMenuButtons(exitButton, sf::String(L"Çıkış"));
    sf::Vector2f playPos;
    playPos = (sf::Vector2f(this->windowSize.x, this->windowSize.y) - this->playButton.getSize()) / 2.f;
    playPos.y += 3 * this->playButton.getSize().y;
    this->playButton.setPosition(playPos);
    this->exitButton.setPosition(playPos.x, playPos.y + this->playButton.getSize().y + 10);
    this->titleText.setFont(font);
    this->titleText.setString("60 Saniyede Tarih");
    this->titleText.setCharacterSize(45);
    sf::Vector2f titlePos;
    sf::Vector2f titleTextSize;
    titleTextSize.x = this->titleText.getLocalBounds().width + this->titleText.getLocalBounds().left;
    titleTextSize.y = this->titleText.getLocalBounds().height + this->titleText.getLocalBounds().top;
    titlePos = (sf::Vector2f(this->windowSize.x, this->windowSize.y) - titleTextSize) / 2.f;
    titlePos.y = 100;
    this->titleText.setPosition(titlePos);
}

void MainWindow::updateMainMenu() 
{
    this->playButton.update();
    this->exitButton.update();
    if(this->playButton.isClicked() && lPress)
    {
        this->currentScene = Scene::Ingame;
        this->timerClock.restart();
        return;
    }
    if(this->exitButton.isClicked() && lPress)
    {
        this->window.close();
    }
}

void MainWindow::drawMainMenu() 
{
    this->playButton.draw();
    this->exitButton.draw();
    window.draw(this->titleText);
}

void MainWindow::createEndScreen() 
{
    createEndText();
    this->createMainMenuButtons(this->restartButton, L"Yeniden Oyna");
    this->createMainMenuButtons(this->mainMenuButton, L"Ana Menü");
    this->createMainMenuButtons(this->exitButtonE, L"Çıkış");
    float restartHeight = this->restartButton.getSize().y;
    sf::Vector2f restartPos = (sf::Vector2f(this->windowSize.x, this->windowSize.y) - this->restartButton.getSize()) / 2.f;
    restartPos.y += 2 * restartHeight;
    this->restartButton.setPosition(restartPos);
    this->mainMenuButton.setPosition(restartPos.x, restartPos.y + restartHeight + 10);
    this->exitButtonE.setPosition(restartPos.x, restartPos.y + 20 + 2 * restartHeight);
}

void MainWindow::updateEndScreen() 
{
    this->restartButton.update();
    this->mainMenuButton.update();
    this->exitButtonE.update();  

    if(this->restartButton.isClicked() && lPress)
    {
        this->currentScene = Scene::Ingame;
        createIngame();
        this->score = 0;
        this->timerClock.restart();
    }
    if(this->exitButtonE.isClicked() && lPress)
        this->window.close();
    if(this->mainMenuButton.isClicked() && lPress)
    {
        this->currentScene = Scene::MainMenu;
        createIngame();
        this->score = 0;
        this->timerClock.restart();
    }
}

void MainWindow::drawEndScreen() 
{
    for(int i = 0; i < 3; i++)
        window.draw(this->endScreenText[i]);
    this->restartButton.draw();
    this->mainMenuButton.draw();
    this->exitButtonE.draw();
}

void MainWindow::createEndText() 
{
    if(score > highScore)
    {
        highScore = score;
        saveHighscore();
    }
    for(int i = 0; i < 3; i++)
        this->endScreenText[i].setFont(this->font);
    this->endScreenText[0].setString(L"Süre Bitti!");
    this->endScreenText[1].setString(L"Skorun: " + std::to_wstring(score));
    this->endScreenText[2].setString(L"En Yüksek Skor: " + std::to_wstring(highScore));
    for(int i = 0; i < 3; i++)
    {
        this->endScreenText[i].setPosition(
        (this->windowSize.x - this->endScreenText[i].getLocalBounds().width) / 2.f + this->endScreenText[i].getLocalBounds().left,
            96 + i * this->endScreenText[i].getLocalBounds().height + i * 10
        );
    }
}

void MainWindow::createQuestions() 
{
    this->questions.push_back(std::make_unique<Question>(
        L"Medine şehri \"Yesrib\" olarak anılırken sonrasında bu şehre\n\"Medinetü'n-Nebi\" denilmesinin nedeni nedir?",
        L"I. Akabe Biatı",
        L"Hicret",
        L"Kerbela Olayı",
        L"Cemel Vakası",
        Answer::B)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Bucaş ve Zoravna Antlaşmalarıyla Ukrayna hangi millete bırakılmıştır?",
        L"Ruslar",
        L"Lehler",
        L"Kazaklar",
        L"Venedikliler",
        Answer::C)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Rönesans hangi ülkede başlamıştır?",
        L"İtalya",
        L"Almanya",
        L"İspanya",
        L"Fransa",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlıda ilk bağımsızlığını kazanan millet hangisidir?",
        L"Yunanlar",
        L"Kırımlılar",
        L"Arnavutlar",
        L"Sırplar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Şark meselesi kim tarafından ortaya atılmıştır?",
        L"Rusya",
        L"İngiltere",
        L"Fransa",
        L"Osmanlı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yazılı metinlerde kullanılan dilin lisanını çözen bilim dalı hangisidir?",
        L"Filoloji",
        L"Nümizmatik",
        L"Epigrafya",
        L"Antropoloji",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Türkiye Selçuklu Devleti'nde deniz ve donanma işlerine bakan en üst yetkili\nkimdir?",
        L"Emir'üs Sevahil",
        L"Emir'ül Umera",
        L"Emir-i Dad",
        L"Pervaneci",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlıda kale duvarları ve surları yıkmak için özel eğitim gören askeri\nbirim hangisidir?",
        L"Lağımcılar",
        L"Humbaracılar",
        L"Tımarlı Sipahiler",
        L"Müsellemler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Kitab-ı Bahriye'nin yazarı kimdir?",
        L"Piri Reis",
        L"Seyid Ali Reis",
        L"Lütfü Paşa",
        L"İbni Kemal",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"İstanbul'un yönetiminden, disiplininden ve güvenliğinden hangi devlet adamı\nsorumludur?",
        L"Veziriazam",
        L"Nişancı",
        L"Kazasker",
        L"Beylerbeyi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı Tarihinde 1718-1730 yılları arasında geçen dönem hangisidir?",
        L"Lâle Devri",
        L"Fetret Devri",
        L"Meşrutiyet Devri",
        L"Tanzimat Devri",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"\"Bir ülke ne kadar madene ve paraya sahipse o kadar zengindir\" diyen anlayış\nhangisidir?",
        L"Merkantalizm",
        L"Emperyalizm",
        L"Makyavelizm",
        L"Rasyonalizm",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"\"Ütopya\" adlı eserin yazarı kimdir?",
        L"Thomas More",
        L"Machiavelli",
        L"J.J. Rousseau",
        L"Spinoza",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Bugünkü Türkiye-İran sınırını büyük ölçüde belirleyen antlaşma hangisidir?",
        L"Kasr-ı Şirin",
        L"Nasuh Paşa",
        L"Ferhat Paşa",
        L"Serav",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Coğrafi Keşifler Osmanlı Devleti'ni öncelikle hangi alanda etkilemiştir?",
        L"Ekonomik",
        L"Siyasi",
        L"Askeri",
        L"Sosyal",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Lâle Devri'ni sona erdiren isyan aşağıdakilerden hangisidir?",
        L"Patrona Halil İsyanı",
        L"Ahmet Paşa İsyanı",
        L"Kabakçı Mustafa İsyanı",
        L"Karayazıcı İsyanı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Basmacı Haraketi'ne destek veren Osmanlı devlet adamı kimdir?",
        L"Enver Paşa",
        L"Cemal Paşa",
        L"Talat Paşa",
        L"Yusuf Akçura",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"I. Dünya Savaşı'nda imzalanan fakat yürürlülüğe girmeyen antlaşma\nhangisidir?",
        L"Sevr Antlaşması",
        L"Versay Antlaşması",
        L"Sen Germen Antlaşması",
        L"Nöyyi Antlaşması",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"II. Dünya Savaşı'ndan önce izafiyet teorisi ile fizik biliminde çığır açan\nbilim insanı kimdir?",
        L"Albert Einstein",
        L"Thomas Bohr",
        L"Thomas Morgan",
        L"Frank Whittle",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Türkiye'nin asker göndererek NATO'ya girmeyi amaçladığı savaş hangisidir?",
        L"Kore Savaşı",
        L"Vietnam Savaşı",
        L"Arap-İsrail Savaşları",
        L"Körfez Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"ABD Ulusal Havacılık ve Uzay Dairesinin olarak faaliyet gösteren kuruluş\nedir? ",
        L"NASA",
        L"FBI",
        L"CIA",
        L"NSA",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Kıbrıs Sorunu'nun çözümü için çalışmalar yapmış Birleşmiş Milletler Genel\nSekreteri kimdir?",
        L"Kofi Annan",
        L"Bonki Moon",
        L"Christine Lagarde",
        L"Vladmir Putin",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Almanya'daki habsburg soyunun Alman prensleriyle yaptığı savaş nedir?",
        L"Otuz Yıl Savaşları",
        L"Kresy Savaşı",
        L"Çifte Gül Savaşı",
        L"Yüzyıl Savaşları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Reform haraketlerinin düşünce ve fikir alanındaki sonucu nedir?",
        L"Skolastik düşünce zayıfladı",
        L"Yeni mezhepler ortaya çıktı",
        L"Mezhep kavgaları başladı",
        L"Feodal yönetim zayıfladı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki isimlerden hangisi Rönesans'ı Fransa'da temsil etmemiştir?",
        L"Erasmus",
        L"Montaigne",
        L"Pierre Lescot",
        L"Ronsard",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Rönesans'ın sebepleri arasında aşağıdakilerden hangisi yer almaz?",
        L"Mezhep savaşlarının başlaması",
        L"Skolastik düşüncenin zayıflaması",
        L"Matbaanın geliştirilmesi",
        L"Antik dönem eserlerinin incelenmesi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yirmi Sekiz Mehmet Çelebi tarafından yazılan, Osmanlı'nın Batı'ya açılan\nilk penceresi olarak kabul edilen eser hangisidir?",
        L"Sefaretname",
        L"Risale",
        L"Mektubat",
        L"Salname",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı Devleti'nin Rusya ile mücadelesinde tampon devlet olarak gördüğü\ndevlet hangisidir?",
        L"Lehistan",
        L"İran",
        L"Eflak",
        L"İsveç",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"II. Mahmut döneminde, merkezi otoriteyi yeniden sağlamak için ayanlarla\nyapılan sözleşme hangisidir?",
        L"Sened-i İttifak",
        L"El-Ariş",
        L"Balta Limanı",
        L"Aynalıkavak",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisinin Mısır sorunu ile bir ilgisi yoktur?",
        L"Edirne Antlaşması",
        L"Londra Antlaşması",
        L"Nizip Savaşı",
        L"Vehhabilik Haraketi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yunanistan'ın kuruluşu hangi antlaşmayla sağlanmıştır?",
        L"Edirne Antlaşması",
        L"Berlin Antlaşması",
        L"Kütahya Antlaşması",
        L"Ziştovi Antlaşması",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki isyanlardan hangisi Ermeniler tarafından başlatılmamıştır?",
        L"Mora İsyanı",
        L"Adana Olayları",
        L"Sason İsyanı",
        L"Van İsyanı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı Devleti Kırım'ın Rusya'ya ait olduğunu kabul ettiği antlaşma\nhangisidir?",
        L"Yaş Antlaşması",
        L"Ziştovi Antlaşması",
        L"Bükreş Antlaşması",
        L"Belgrad Antlaşması",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"\"Şark Meselesi\" kavramı ilk kez nerede siyasi bir terim olarak\nkullanılmıştır?",
        L"Viyana Kongresi",
        L"Paris Konferansı",
        L"Berlin Konferansı",
        L"İstanbul Konferansı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Rusya ile Tilsit Antlaşması'nı imzalayan Avrupa devleti hangisidir?",
        L"Fransa",
        L"Avusturya",
        L"İngiltere",
        L"Almanya",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı Devleti için ilk kez \"Hasta Adam\" ifadesini kullanan devlet\nhangisidir?",
        L"Rusya",
        L"Almanya",
        L"Fransa",
        L"Prusya",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yunan isyanını bastırmasından sonra Mora ve Girit Valiliklerini isteyen\nMısır Valisi kimdir?",
        L"Mehmet Ali Paşa",
        L"Alemdar Mustafa Paşa",
        L"Tepedelenli Ali Paşa",
        L"Selim Paşa",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi nüfus atışlarının kazançlarından biri değildir?",
        L"İç ve Dış göçler",
        L"Savaş gücünün artması",
        L"Vergi gelirlerinin artması",
        L"Üretim gelirlerinin artması",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"XVII. yüzyılda Avrupa'da kol gücünün yerini makine gücünün alması ile\nbaşlayan değişimin adı nedir?",
        L"Sanayi İnkılabı",
        L"Akıl Çağı",
        L"Fransız İhtilali",
        L"Helenistik Çağ",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı'nın son dönemlerinde sanayileşme çabalarının başarısız\nolmasının en önemli nedeni nedir?",
        L"Teknolojik gelişmeleri takip etmemek",
        L"Yebiçeri isyanları",
        L"Ham maddenin azlığı",
        L"Madenlerin yetersizliği",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi Osmanlı Devleti'nin sanayisi geliştirmek için\naldığı önlemlerdendir?",
        L"Ham madde satışını sınırlamak",
        L"Avrupa'dan ucuz mal almak",
        L"Ekonomik atılım gerçekleştirmek",
        L"Kapitülasyınları sınırlamak",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi kadınlara yönelik yayın yapan gazete ve\ndergilerden değildir?",
        L"Tercüman-ı Hakikat",
        L"Şükufezar",
        L"Terakki Gazetesi",
        L"Parça Bohçası",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi sanayileşmenin olumsuz sonuçlarından biri\ndeğildir?",
        L"Kültürel faaliyetlerin artması",
        L"Hava kirliliği",
        L"Su sıkıntısı",
        L"Şehirlerin çarpık gelişmesi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi tüketim ekonomisinin ortaya çıkardığı yeni iş\nalanlarındandır?",
        L"Reklamcılık",
        L"Esnaflık",
        L"Dokumacılık",
        L"Zanaatkârlık",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki şehirlerden hangisi Türkiye Selçukluları tarafından\nfethedilmemiştir?",
        L"Edirne",
        L"Konya",
        L"Sivas",
        L"Kayseri",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi Malazgirt Savaşı'ndan sonra Anadolu'da\nkurulan ilk dönem beylikler arasında yer almaz?",
        L"Karamanoğulları",
        L"Saltuklular",
        L"Artuklular",
        L"Çaka Beyliği",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi Büyük Selçuklu topraklarında kurulan\natabeyliklerden değildir?",
        L"Saltuklular",
        L"Salgurlular",
        L"Zenginoğulları",
        L"Börililer",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Türk İslam devletlerinde saray teşkilatında görev almaz?",
        L"Muhtesip",
        L"Hacip",
        L"Camedar",
        L"Emir-i Silah",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Divanıhümayunun üyeleri arasında aşağıdakilerden hangisi\ngösterilemez?",
        L"Taht Kadısı",
        L"Nişancı",
        L"Kazasker",
        L"Müftü",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı'da devşirme kökenli devlet adamlarının yetiştirildiği ve saray\nokulu olarak adlandırılan bölümün adı nedir?",
        L"Enderun",
        L"Birun",
        L"İtizam",
        L"Kapan",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"I. Dünya Savaşı sonrası yeni kurulan devletler arasında aşağıdakilerden\nhangisi yer almaz?",
        L"Romanya",
        L"Macaristan",
        L"Yugoslavya",
        L"Polonya",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Kösedağ Savaşı hangi iki devlet arasında yapılmıştır?",
        L"Moğollar - Türkiye Selçukluları",
        L"Eyyübiler - Haçlılar",
        L"Memlükler - Bizanslılar",
        L"Harzemşahlar - Moğollar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki devletlerden hangisi Anadolu'da Türkçeyi resmi devlet dili\nolarak kullanmıştır?",
        L"Karamanoğulları",
        L"Danişmentliler",
        L"Türkiye Selçuklu Devleti",
        L"Karesioğulları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yağıbasan Medresesi hangi beyliğe aittir?",
        L"Danişmentliler",
        L"Artukoğulları",
        L"Mengücekler",
        L"Saltuklular",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"1096-1099 yılları arasında yola çıkan Haçlı ordusu Kudüs'ü hangi devletin\nelinden almıştır?",
        L"Fâtımîler",
        L"Musul Atabeyliği",
        L"Eyyubiler",
        L"Memlûkler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Adaletin temsilcisi olan Osmanlı idarecisi hangisidir?",
        L"Kadı",
        L"Subaşı",
        L"Şeyhülislam",
        L"Muhtesip",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı'da kamuya hizmet amacıyla yapılan mimari eserler arasında hangisi\nyer almaz?",
        L"Kümbet",
        L"Dârülkurrâ",
        L"Kervansaray",
        L"Darüşşifa",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Osmanlı Erken ve Klasik Dönemi'nde gelişme gösteren sanat\ndallarından biri değildir?",
        L"Heykel",
        L"Hat",
        L"Dokumacılık",
        L"Çinicilik",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki camilerden hangisi osmanlıda yer almaz?",
        L"Selimiye Cami",
        L"Süleymaniye Cami",
        L"Beyazid Cami",
        L"Şehzade Cami",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi halkın eğlenmesine imkân sağlayan saray merasimlerinden biri\ndeğildir?",
        L"Cuma selamlığı",
        L"Cülus Törenleri",
        L"Hanedan ailesi düğünleri",
        L"Askeri zaferler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi kuruluş döneminde, Anadolu'da Türk siyasi birliğini sağlamak\namacıyla ele geçirilen yerlerden biridir?",
        L"Kütahya",
        L"İzmit",
        L"İznik",
        L"Bursa",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi kuruluş döneminde Osmanlı'nın Balkanlarda hakimiyet kurmak\niçin yaptığı savaşlardan biri değildir?",
        L"Koyunhisar",
        L"Varna",
        L"I. Kosova",
        L"II. Kosova",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi savaşta Bizans, Osmanlı'ya yenilerek Haçlılardan yardım istemiştir?",
        L"Maltepe Savaşı",
        L"Çimen Savaşı",
        L"Niğbolu Savaşı",
        L"Sırpsındığı Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı'nın Balkanları Türk vatanı yapmak için takip ettiği siyaset\nhangisidir?",
        L"İskan siyaseti",
        L"Fetih siyaseti",
        L"Barış siyaseti",
        L"Yayılma siyaseti",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı'nın topraklarına kattığı ilk Anadolu Türk beyliği hangisidir?",
        L"Karesioğulları",
        L"Germiyanoğulları",
        L"Hamitoğulları",
        L"Candaroğulları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki savaşların hangisinde Haçlı İttifakı'na karşı Osmanlı'nın\nyanında diğer anadolu beylikleri de yer almıştır?",
        L"I. Kosova",
        L"Çirmen",
        L"Sazlıdere",
        L"Koyunhisar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi XIV. yüzyıl başlarında Yakın Doğu ve Avrupa'da bulunan\ndevletlerden değildir?",
        L"Büyük Selçuklu Devleti",
        L"İlhanlılar",
        L"Memlüklüler",
        L"Altın Orda Devleti",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Osmanlı'nın Balkanlardaki fetih faaliyetlerini hızlandıran\nsavaşlar arasında yer almaz?",
        L"Ankara Savaşı",
        L"Sazlıdere Savaşı",
        L"I. Kosova Savaşı",
        L"I. Çimen Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi beylik Osmanlı tarafından Anadolu Türk Siyasi birliğini sağlamak\namacı ile sonlandırılmamıştır?",
        L"Karakoyunlar",
        L"Karesioğulları",
        L"Aydınoğulları",
        L"Germiyanoğulları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlıdaki ilk dini ve sosyal boyutlu isyanın adı nedir?",
        L"Şeyh Bedrettin İsyanı",
        L"Buçuktepe İsyanı",
        L"Düzmece Mustafa Olayı",
        L"Şehzade Mustafa Olayı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Osmanlı'da tarımsal üretimin devamlılığını sağlayan sistemdir?",
        L"Tımar sistemi",
        L"Vergi sistemi",
        L"Toprak sistemi",
        L"Millet sistemi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Osman Bey'in silah arkadaşlarından biri değildir?",
        L"Alp Arslan",
        L"Saltuk Alp",
        L"Konur Alp",
        L"Akça Koca",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Orhan Bey dönemi kurulan ilk düzenli ordu hangisidir?",
        L"Yaya ve müsellemler",
        L"Yeniçeri Ocağı",
        L"Acemi Ocağı",
        L"Tımarlı Sipahiler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yeniçeri Ocağının kurulmasından sonra öncü birlik olarak kullanılan\nbirlik hangisidir?",
        L"Azebler",
        L"Tımarlı Sipahiler",
        L"Gaziyân-ı Rûm",
        L"Yaya ve müsellemler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Altı Bölük olarak da isimlendirilen Osmanlı askerî birliği\nhangisidir?",
        L"Kapıkulu Süvariler",
        L"Tımarlı Sipahiler",
        L"Cebeci Ocağı",
        L"Yayalar ve müsellemler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi Osmanlı askeri birliği Türk-İslam devletlerindeki gulam ordusuna\nbenzer?",
        L"Kapıkulu Askerleri",
        L"Sakacılar",
        L"Akıncılar",
        L"Müsellemler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Yeniçeri ağasının ocakla ilgili işleri görmek üzere kurduğu divan hangisidir?",
        L"Ağa Divânı",
        L"İkindi Divânı",
        L"Ayak Divânı",
        L"Galeve Divânı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Devşirme yoluyla alınan çocuklara askerî eğitim ilk hangi birimde\nverilir?",
        L"Acemi Ocağı",
        L"Sipahiler",
        L"Humbaracılar",
        L"Cebeci Ocağı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Acemi ocağında eğitilen devşirmelerin Yeniçeri Ocağına alınmalarına\nne ad verilir?",
        L"Kapıya Çıkma",
        L"Sancağa Çıkma",
        L"Kafes Usülü",
        L"Pençik Sistemi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki tarikatlardan hangisi Anadolu'nun islamlaşmasında etkin\nbir rol almamıştır?",
        L"Vehhabilik",
        L"Rufailik",
        L"Yesevilik",
        L"Bektaşilik",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdakilerden hangisi Seyfiye sınıfının temel görevleri arasındadır?",
        L"Askerlik",
        L"Bürokrasi",
        L"Sağlık",
        L"Eğitim",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Mevlana, Yunus Emre ve Hacı Bektaş Veli ile başlayan edebiyat akımı\nhangisidir?",
        L"Tekke Edebiyatı",
        L"Divan Edebiyatı",
        L"Aşık Edebiyatı",
        L"Halk Edebiyatı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi ilmiye sınıfan mensup değildir?",
        L"Reisülküttab",
        L"Müderris",
        L"Şeyhülislam",
        L"Kazasker",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Aşağıdaki mutasavvıflardan hangisi Anadolu'da yaşamamıştır?",
        L"Ahmet Yesevi",
        L"Ahî Evran",
        L"Hacı Bektâş-ı Velî",
        L"Hacı Bayram-ı Velî",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangiye Seyfiye sınıfına mensup değildir?",
        L"Defterdar",
        L"Vezirler",
        L"Beylerbeyi",
        L"Sancakbeyi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Fatih Sultan Mehmet'in Bizans'ın yeniden kurulmasını önlemek için\ntopraklarına kattığı yer neresidir?",
        L"Mora Rum Despotlukları",
        L"Arnavutluk Prensliği",
        L"Eflay Voyvodalığı",
        L"İsfendiyaroğulları Beyliği",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı İstanbul'u fethedip ticaret yolları üzerindeki denetimini\narttırınca Avrupalılar ne yapmıştır?",
        L"Coğrafi keşifler",
        L"Hristiyan birliği",
        L"Gümrük birliği",
        L"Skolastik düşünce",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"I. Elizabeth'in ittifak arayışında olduğu \"Büyük Türk\" hangi Osmanlı\npadişahıdır?",
        L"I. Süleyman",
        L"II. Mehmet",
        L"II. Bayezid",
        L"I. Selim",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlı tarihinin yazılmasını ilk emreden padişah kimdir?",
        L"II. Bayezit",
        L"II. Murat",
        L"II. Mehmet",
        L"I. Murat",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Birun'da yer almaz?",
        L"Beylerbeyi",
        L"Topçular",
        L"Mehterler",
        L"Çavuşlar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Osmanlıda mülkiyeti devlete ait olan topraklara ne ad verilir?",
        L"Mirî arazi",
        L"Öşri arazi",
        L"Vakıf arazi",
        L"Mülk arazi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi mirî arazisinin bölümlerinden biridir?",
        L"Dirlik",
        L"Mukataa",
        L"Ocaklık",
        L"Yurtluk",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Müslüman çiftçilerden alınan ürün vergisine ne ad verilir?",
        L"Öşür",
        L"Cizye",
        L"Çiftbozan",
        L"Adetiağnam",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi vakıfların faaliyet alanları arasında yer almaz?",
        L"Yeniçerilere ulufe vermek",
        L"Köprü yapmak",
        L"Cami yapmak",
        L"Aşevleri kurmak",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Orta Asya göçlerinin sebepleri arasında yer almaz?",
        L"Ticareti geliştirme isteği",
        L"Aşırı nüfus artışı",
        L"Kuraklık",
        L"Hayvan hastalıkları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Asya Hun Devleti hakkındaki ilk bilgileri hangi kaynak vermektedir?",
        L"Çin kaynakları",
        L"Orhun anıtları",
        L"Arkeolojik kazılar",
        L"Mezar taşları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi kurultay ile ilgili yanlış bir bilgidir?",
        L"Hatun başkanlık yapabilir",
        L"Üyelerine Toygun denir",
        L"Yılda en az üç kez toplanır",
        L"Boy beyleri de katılır",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Asya Hun Devleti ile Çin arasındaki mücadelenin temel sebebi nedir?",
        L"İpek yolu hâkimiyeti",
        L"Siyasi birliği sağlama",
        L"Kürk yolu egemenliği",
        L"Tarımsal alanları ele geçirme",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"\"Türk\" adından ilk olarak hangi metinde bahsedilmiştir?",
        L"Çin yıllıklarında",
        L"İran belgelerinde",
        L"Yenisey Yazıtlarında",
        L"Kök Türk Kitabelerinde",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"İslamiyet öncesi kurulan Türk devletlerinin ikiye bölünmesi neyin\nsonucudur?",
        L"İkili teşkilat",
        L"Kut anlayışı",
        L"Yarı göçebe yaşam",
        L"Göktanrı inancı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Orta Çağ Avrupa'sında görülen sosyal sınıflardandır?",
        L"Burjuva",
        L"Brahmanlar",
        L"Paryalar",
        L"Sudralar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Doğu Roma İmparatorluğu'nun resmi mezhebi nedir?",
        L"Ortadoksluk",
        L"Protestanlık",
        L"Katoliklik",
        L"Kalvinizm",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Orta Çağ Avrupa'sında Ortadoks mezhebinin merkezi neresidir?",
        L"Konstantinapolis",
        L"Roma",
        L"Viyana",
        L"Belgrat",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"İslamiyet'ten önceki dönemlerde Mekke, Medine ve Taif şehirlerinin\nbulunduğu bölgeye ne ad verilirdi?",
        L"Hicaz",
        L"Yemen",
        L"Necid",
        L"Himyeri",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi gelişmeden sonra mekkeli müşrikler Müslümanların varlığını\nhukuken tanımıştır?",
        L"Hudeybiye Barışı",
        L"Tebük Seferi",
        L"Akabe Biatı",
        L"Medine Sözleşmesi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Müslümanlarla Bizans arasında yapılan ilk savaşın adı nedir?",
        L"Mute Savaşı",
        L"Huneyn Savaşı",
        L"Ecnadin Savaşı",
        L"Kadisiye Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Hz. Osman Döneminde gerçekleşmemiştir?",
        L"Sıffın Savaşı'nın yapılması",
        L"Kur'an-ı Kerim'in çoğaltılması",
        L"Tunus'un ele geçirilmesi",
        L"Kıbrıs'ın vergiye bağlanması",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Muaviye'nin Şam'da halifeliğini ilan etmesi nedeniyle çıkan savaş nedir?",
        L"Sıffın savaşı",
        L"Cemel Savaşı",
        L"Yermük Savaşı",
        L"Ecnadin Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Müslümanların Avrupa'da ilerleyişinin durması açısından dönüm noktası\nolan savaşın adı nedir?",
        L"Puvatya Savaşı",
        L"Kadiks Savaşı",
        L"Celula Savaşı",
        L"Kadisiye Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Emevi Devleti'nde ilk paranın bastırılması hangi hükümdarın döneminde\ngerçekleşmiştir?",
        L"Abdülmelik b. Mervân",
        L"Muaviye",
        L"II.Velid",
        L"Yezid",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Endülüs Emevi Devleti'nin kurucusu kimdir?",
        L"Abdurrahman",
        L"Muaviye",
        L"Hişam",
        L"Ömer Bin Abdülaziz",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Endülüs Emevi Devleti'nin başkenti neresidir?",
        L"Kurtuba",
        L"Şam",
        L"Bağdat",
        L"Kufe",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Abbasi Devleti'nde merkezi otoritenin zayıflaması ile Mısır'da ortaya\nçıkan Türk devletinin adı nedir?",
        L"Ihşidler",
        L"Selçuklular",
        L"Karahanlılar",
        L"Gazneliler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"El-Kanun Fi't-Tıb adlı eseri yazan Türk bilim insanı kimdir?",
        L"İbn-i Sina",
        L"Farabi",
        L"El-Biruni",
        L"Gazali",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"\"2. Öğretmen\" lakabıyla anılan İslam alimi kimdir?",
        L"Farabi",
        L"İbn-i Sina",
        L"İmam-ı Azam",
        L"İmam Gazali",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Emevilerin yıkılması nedenleri arasında gösterilemez?",
        L"Türkler'in İslamiyeti kabulü",
        L"Fetihlerin durması",
        L"Şîîlerin faaliyetleri",
        L"Arap milliyetçiliği politikası",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Mutluluk veren bilgi anlamına gelen eser hangisidir?",
        L"Kutadgu Bilig",
        L"Divan-ı Lügat'it Türk",
        L"Atabet-ül Hakayık",
        L"Divan-ı Hikmet",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Büyük Selçuklu ile Bizans arasında yapılan ilk savaş hangisdir?",
        L"Pasinler Savaşı",
        L"Miryokefalon Savaşı",
        L"Malazgirt Savaşı",
        L"Katvan Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Büyük Selçuklu'nun yıkılmasıyla ortaya çıkmamıştır?",
        L"Mısır Selçukluları",
        L"Türkiye Selçukluları",
        L"Suriye Selçukluları",
        L"Kirman Selçukluları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Romen Diyojen'in Bizans'ı Türk tehlikesinden korumak için yaptığı savaş\nhangisidir?",
        L"Malazgirt Savaşı",
        L"Miryokefalon Savaşı",
        L"Hittin Savaşı",
        L"Pasinler Savaşı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"İlk Türk gramer kitabi ve ilk Türkçe ansiklopedik sözlük olarak da kabul\nedilen eserin adı nedir?",
        L"Divan-ı Lügat'it Türk",
        L"Kutadgu Bilig",
        L"Şehname",
        L"Atabet-ül Hakayık",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Karahanlıların Doğu ve Batı Karahanlılar olarak ikiye ayrılmasının sebebi\nnedir?",
        L"Taht kavgaları",
        L"Oğuz isyanları",
        L"Gazneli saldırıları",
        L"Çin entrikaları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Galileo'dan 600 sene önce dünyanın döndüğü fikrini savunan bilgin\nkimdir?",
        L"Biruni",
        L"Gazali",
        L"Harezmî",
        L"Farabi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Büyük Selçuklularda şehzadelerin eğitimi veya bağımsız olarak bir\neyaletin yönetimi ile görevli vezirlere ne ad verilir?",
        L"Atabey",
        L"Lala",
        L"Kadi-I Kudat",
        L"Hacip",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Büyük selçukluların yıkılma nedenleri arasında gösterilemez?",
        L"Bizans'ın baskısı",
        L"Oğuzların isyanı",
        L"Katvan Savaşı",
        L"Bâtınilerin faaliyetleri",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Büyük Selçuklulara başkentlik yapmamıştır?",
        L"Semerkant",
        L"Merv",
        L"Cend",
        L"Rey",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Kast sistemi hangi medeniyette ortaya çıkmıştır?",
        L"Hint",
        L"Sümer",
        L"Babil",
        L"Mısır",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"1347-1351 yılları arasında Avrupa'nın neredeyse yarısının ölümüne\nsebep olan gelişme nedir?",
        L"Veba salgını",
        L"Doğal afet",
        L"İç isyanlar",
        L"Otuz Yıl Savaşları",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Patrici-Plep arasındaki sınıf mücadelesi sonucu hazırlanan Roma hukuk\nkurallarına ne ad verilir?",
        L"On İki Levha Kanunları",
        L"Solon Kanunu",
        L"Dragon Kanunu",
        L"Psistaros Kanunu",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Dünyanın ilk kentsel yerleşimi olarak kabul edilen şehrin adı nedir?",
        L"Çatalhöyük",
        L"Yarımburgaz",
        L"Laskö",
        L"Beldibi",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Uygarlıkların doğuşunda hangisi etkili değildir?",
        L"Tropikal yağmurlar",
        L"Siyasi durum",
        L"Ekonomi",
        L"Dinsel yapı",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Eski Yunan uygarlıklarında şehir devletlerine verilen isim nedir?",
        L"Polis",
        L"Site",
        L"Nom",
        L"Tekfur",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Tarihin ilk yazılı antlaşması hangi iki devlet arasında gerçekleşmiştir?",
        L"Hitit-Mısır",
        L"İran-Mısır",
        L"Asur-Hitit",
        L"Frigya-Lidya",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Mısır uygarlığında takvimin icat edilmesindeki en etkili olay nedir?",
        L"Nil Nehri'nin taşması",
        L"Güneş tutulması",
        L"Ay tutulması",
        L"Mimarideki ilerlemeler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi uygarlık bugünkü modern alfabenin temelini atmıştır?",
        L"Fenikeliler",
        L"Yunanlılar",
        L"Urartular",
        L"Romalılar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Orta Asya kültür merkezleri arasında hangisi yer almaz?",
        L"Truva",
        L"Tagar",
        L"Afanesyevo",
        L"Karasuk",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Bozkırın Kuyumcuları olarak da bilinen Türk topluluğunun adı nedir?",
        L"Sakalar",
        L"Hunlar",
        L"Köktürkler",
        L"Uygurlar",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Anadolu'da ilk siyasal birliği kuran uygarlık hangisidir?",
        L"Hititler",
        L"Hattiler",
        L"Sümerler",
        L"Frigler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Anadolu'da kurulan İlk Çağ medeniyetlerinden değildir?",
        L"Akadlar",
        L"Urartular",
        L"Frigler",
        L"Hititler",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangisi Mısır uygarlığına ait değildir?",
        L"Ziggurat",
        L"Tanrı Kral",
        L"Firavun",
        L"Hiyeroglif",
        Answer::A)
    );
    this->questions.push_back(std::make_unique<Question>(
        L"Hangi uygarlık hukuk kurallarını ilk kez yazılı hâle getirmiştir?",
        L"Sümerler",
        L"Babiller",
        L"Asurlar",
        L"Persler",
        Answer::A)
    );

    //std::cout << this->questions.size() << " questions." << std::endl;
}

void MainWindow::selectQuestion(std::size_t index)
{
    this->questions[index]->shuffleAnswers(25);
    this->questionField.setString(this->questions[index]->getQuestion());
    this->optionA.setString(L"A) " + this->questions[index]->getAnswer(Answer::A));
    this->optionB.setString(L"B) " + this->questions[index]->getAnswer(Answer::B));
    this->optionC.setString(L"C) " + this->questions[index]->getAnswer(Answer::C));
    this->optionD.setString(L"D) " + this->questions[index]->getAnswer(Answer::D));
}

void MainWindow::pickedRight() 
{
    score += 2;
    scoreText.setString("Skorun: " + std::to_string(score));
    startGrading(true);
}

void MainWindow::pickedWrong() 
{
    score --;
    scoreText.setString("Skorun: " + std::to_string(score));
    startGrading(false);
}

void MainWindow::saveHighscore() 
{
    std::ofstream ofs(filePath, std::ios::binary);
    ofs.write((char*)&highScore, sizeof(std::int64_t));
    ofs.close();
}

std::size_t MainWindow::generateRandomIndex() 
{
    std::random_device device;
    std::mt19937 gen(device());
    std::uniform_int_distribution<std::size_t> dist(0, questions.size() - 1);
    return dist(gen);
}

void MainWindow::createScoreText() 
{
    scoreText.setString("Skorun: 0");
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
}

void MainWindow::gradeFunction() 
{
    if(isGrading)
    {
        if(!isBackwards)
        {
            distanceGraded += gradingSpeed;
            if(distanceGraded >= 1)
            {
                distanceGraded = 1;
                isBackwards = true;
            }
            backgroundColor = colorGradiant(originalBackgroundColor, targetColor, distanceGraded);
        }
        else
        {
            distanceGraded -= gradingSpeed;
            if(distanceGraded <= 0)
            {
                distanceGraded = 0;
                isBackwards = false;
                isGrading = false;
            }
            backgroundColor = colorGradiant(originalBackgroundColor, targetColor, distanceGraded);
        }
    }
}

void MainWindow::startGrading(bool toGreen) 
{
    isGrading = true;
    distanceGraded = false;
    isBackwards = false;
    if(toGreen) targetColor = color_right;
    else targetColor = color_wrong;
}
