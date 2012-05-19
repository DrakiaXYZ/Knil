#include "Game.NPC1.h"


GameNPC1::GameNPC1() {
    x = block.x = 352;
    y = block.y = 368;
    w = block.w = 32;
    h = block.h = 48;
}

GameNPC1::~GameNPC1() {
    if (Surface)
        SDL_FreeSurface(Surface);
}

bool GameNPC1::Init(GameDialog* d, GameCharacter* p) {
    dialog = d;
    player = p;

    Surface = GameDraw::LoadImage("Images/GoodGuy.png", SDL_MapRGB(screen->format, 0xff, 0, 0xff));

    return true;
}

void GameNPC1::Collide() {
    // Set qvar 0 when item obtained
    if (!player->getQVar(0)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nWelcome to The Adventures of Knil.\n\nThis is a demo level in which you will complete several puzzles to\nproceed.");
        dialog->addPage("GoodGuy\n\nSome goons have kidnapped the princess!\nWe need you to get her back Knil!");
        dialog->addPage("GoodGuy\n\nPlease go into my house and get me my [Item]");
        dialog->ShowPage(0);
    }
    // Set qvar 1 when item acknowledged
    if (player->getQVar(0) && !player->getQVar(10)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nThank you.\nCould you please go to the basement and get me my [Item2]?\nSorry about the mess ^.^'");
        dialog->ShowPage(0);
        player->setQVar(1, 1);
    }
    // Set qvar 11 when item2 acknowledged
    if (player->getQVar(0) && player->getQVar(10) && !player->getQVar(20)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nThank you.\nCould you please go west into the forest and get me [Item3]?\nIt's hidden in the trees!");
        dialog->ShowPage(0);
        player->setQVar(11, 1);
    }
    // Set qvar 21 when item3 acknowledged
    if (player->getQVar(0) && player->getQVar(10) && player->getQVar(20) && !player->getQVar(21)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nThank you!\nI now have all the items I need to get us to where the\nprincess is being held.\nHold on tight!");
        dialog->addPage("GoodGuy\n\nActually, that's it for this demo. Thanks for playing!\nTime constraints have limited the amount of content I could create\nIt is now 1am x.x Class in t-minus 7 hours and counting");
        dialog->ShowPage(0);
        player->setQVar(21, 1);
    } else if (player->getQVar(21) && !player->getQVar(22)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nThere's not much else to this demo, you can stop playing now :)");
        dialog->ShowPage(0);
        player->setQVar(22, 1);
    } else if (player->getQVar(22) && !player->getQVar(23)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nOk, ok, here's a cow...");
        dialog->addPage(DIALOGCOW);
        dialog->ShowPage(0);
        player->setQVar(23, 1);
    } else if (player->getQVar(23)) {
        dialog->clearPages();
        dialog->addPage("GoodGuy\n\nOk, that is seriously all there is!");
        dialog->ShowPage(0);
    }
}

void GameNPC1::Step(int dt) {
}
