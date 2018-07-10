#include "Configure_Settings_Form.h"
#include "ui_Configure_Settings_Form.h"
#include "../../Level-Headed/Common_Strings.h"
#include "../SMB1_Writer/SMB1_Writer_Strings.h"
#include "Difficulty_Level_Configurations.h"
#include <QCryptographicHash>
#include <QFileDialog>
#include <QFileInfo>
#include <QTime>
#include <QMessageBox>
#include <QDir>
#include <assert.h>

Configure_Settings_Form::Configure_Settings_Form(QWidget *parent, Plugin_Settings *pluginSettings, SMB1_Writer_Interface *writerPlugin, const QString &location) :
    QDialog(parent),
    ui(new Ui::Configure_Settings_Form)
{
    assert(pluginSettings);
    assert(writerPlugin);
    this->parent = parent;
    this->applicationLocation = location;
    this->pluginSettings = pluginSettings;
    this->writerPlugin = writerPlugin;
    this->levelLocation = location + "/" + Common_Strings::STRING_LEVELS + "/" + Common_Strings::STRING_GAME_NAME;
    this->setWindowFlags(this->windowFlags() & ~Qt::WindowContextHelpButtonHint);

    QDir dir(location + "/" + Common_Strings::STRING_LEVELS);
    if (!dir.exists(this->levelLocation)) dir.mkdir(this->levelLocation); //don't bother checking for success here

    this->numWorlds = this->pluginSettings->numWorlds;
    this->numLevelsPerWorld = this->pluginSettings->numLevelsPerWorld;
    this->randomNumWorlds = this->pluginSettings->randomNumWorlds;

    //Prepare the Difficulty Settings
    this->difficultyComboIndex = this->pluginSettings->difficultyComboIndex;
    this->difficultySettings.save = false;
    this->difficultySettings.bulletTime = this->pluginSettings->difficultyBulletTime;
    this->difficultySettings.hammerTime = this->pluginSettings->difficultyHammerTime;
    this->difficultySettings.buzzyBeetlesReplaceLoneGoombas = this->pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas;
    this->difficultySettings.bridgeFlyingCheepCheeps = this->pluginSettings->difficultyBridgeFlyingCheepCheeps;
    this->difficultySettings.bridgeLakitus = this->pluginSettings->difficultyBridgeLakitus;
    this->difficultySettings.bridgeOffscreenBulletBills = this->pluginSettings->difficultyBridgeOffscreenBulletBills;
    this->difficultySettings.castleFlyingCheepCheeps = this->pluginSettings->difficultyCastleFlyingCheepCheeps;
    this->difficultySettings.castleLakitus = this->pluginSettings->difficultyCastleLakitus;
    this->difficultySettings.castleOffscreenBulletBills = this->pluginSettings->difficultyCastleOffscreenBulletBills;
    this->difficultySettings.islandFlyingCheepCheeps = this->pluginSettings->difficultyIslandFlyingCheepCheeps;
    this->difficultySettings.islandLakitus = this->pluginSettings->difficultyIslandLakitus;
    this->difficultySettings.islandOffscreenBulletBills = this->pluginSettings->difficultyIslandOffscreenBulletBills;
    this->difficultySettings.undergroundFlyingCheepCheeps = this->pluginSettings->difficultyUndergroundFlyingCheepCheeps;
    this->difficultySettings.undergroundLakitus = this->pluginSettings->difficultyUndergroundLakitus;
    this->difficultySettings.undergroundOffscreenBulletBills = this->pluginSettings->difficultyUndergroundOffscreenBulletBills;
    this->difficultySettings.underwaterBloopers = this->pluginSettings->difficultyUnderwaterBloopers;
    this->difficultySettings.underwaterFlyingCheepCheeps = this->pluginSettings->difficultyUnderwaterFlyingCheepCheeps;
    this->difficultySettings.underwaterLakitus = this->pluginSettings->difficultyUnderwaterLakitus;
    this->difficultySettings.underwaterSwimmingCheepCheeps = this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps;
    this->difficultySettings.standardOverworldFlyingCheepCheeps = this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps;
    this->difficultySettings.standardOverworldLakitus = this->pluginSettings->difficultyStandardOverworldLakitus;
    this->difficultySettings.standardOverworldOffscreenBulletBills = this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills;
    this->difficultySettings.noEnemies = this->pluginSettings->difficultyNoEnemies;
    this->difficultySettings.hammerTimeIntensity = this->pluginSettings->difficultyHammerTimeIntensity;
    this->difficultySettings.lakituSpawnChancePerLevel = this->pluginSettings->difficultyLakituSpawnChancePerLevel;
    this->difficultySettings.disableAllOtherEnemiesWhenALakituSpawns = this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns;
    this->difficultySettings.spawnerPriority = this->pluginSettings->difficultySpawnerPriority;

    ui->setupUi(this);
    this->Populate_Installed_ROMs();
    this->Populate_Graphics_Packs();
    this->Populate_Music_Packs();
    this->ui->cbGenerateNewLevels->setChecked(this->pluginSettings->generateNewLevels);
    this->Enable_New_Level_Options(this->pluginSettings->generateNewLevels);
    this->Load_Settings();
}

Configure_Settings_Form::~Configure_Settings_Form() {
    delete ui;
}


void Configure_Settings_Form::Populate_Installed_ROMs() {
    this->ui->comboBaseROM->clear();
    QStringList installedROMs = this->writerPlugin->Get_Installed_ROMs();
    QString tmpBaseROM = this->pluginSettings->baseROM;
    if (installedROMs.isEmpty()) {
        this->ui->comboBaseROM->addItem(STRING_NO_ROMS_INSTALLED);
    } else {
        this->ui->comboBaseROM->addItems(installedROMs);
    }
    if (tmpBaseROM != "" && installedROMs.contains(tmpBaseROM)) {
        this->ui->comboBaseROM->setCurrentText(tmpBaseROM);
    } else {
        this->ui->comboBaseROM->setCurrentIndex(0);
    }
}

void Configure_Settings_Form::Populate_Graphics_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Graphics_Get_Graphic_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboGraphics->addItems(packs);
}

void Configure_Settings_Form::Populate_Music_Packs() {
    QStringList packsWithExtension = this->writerPlugin->Music_Get_Music_Packs();
    QStringList packs;
    for (int i = 0; i < packsWithExtension.size(); ++i) {
        packs.append(QFileInfo(packsWithExtension.at(i)).completeBaseName());
    }
    this->ui->comboMusic->addItems(packs);
}

void Configure_Settings_Form::Load_Settings() {
    if (!this->pluginSettings->baseROM.isEmpty()) this->ui->comboBaseROM->setCurrentText(this->pluginSettings->baseROM);
    this->ui->leOutputROMLocation->setText(this->pluginSettings->outputROMLocation);
    this->ui->cbOverwriteOutputROM->setChecked(this->pluginSettings->overwriteOuputROM);
    if (this->pluginSettings->music < this->ui->comboMusic->count()) this->ui->comboMusic->setCurrentIndex(this->pluginSettings->music);
    else this->ui->comboMusic->setCurrentIndex(2); //use original music
    this->ui->cbCombineWithOtherMusicPacks->setChecked(this->pluginSettings->combineMusicWithOtherPacks);
    this->ui->comboTone->setCurrentIndex(this->pluginSettings->toneColor);
    if (this->pluginSettings->graphics < this->ui->comboGraphics->count()) this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    else this->ui->comboGraphics->setCurrentIndex(1); //use original graphics
    this->ui->comboGraphics->setCurrentIndex(this->pluginSettings->graphics);
    this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->cbInfiniteLives->setChecked(this->pluginSettings->infiniteLives);
    this->ui->cbGodMode->setChecked(this->pluginSettings->godMode);
    this->ui->cb2PlayerGame->setChecked(!this->pluginSettings->addLuigiGame);
    this->ui->cbRevertToSuperMario->setCheckState(this->pluginSettings->superMarioOnDamage);
    this->ui->cbLakituThrowArc->setCheckState(this->pluginSettings->lakituThrowArc);
    this->ui->cbAutoscroll->setCheckState(this->pluginSettings->autoscroll);
    this->ui->cbReplaceFireFlowerWithHammerSuit->setCheckState(this->pluginSettings->replaceFireFlowerWithHammerSuit);
    this->ui->comboEnemySpeed->setCurrentIndex(this->pluginSettings->enemySpeed-1);
}

void Configure_Settings_Form::Save_Settings() {
    QString baseROM = this->ui->comboBaseROM->currentText();
    if (!baseROM.isEmpty() && baseROM != STRING_NO_ROMS_INSTALLED) this->pluginSettings->baseROM = baseROM;
    if (QFileInfo(this->ui->leOutputROMLocation->text()).absoluteDir().exists()) this->pluginSettings->outputROMLocation = this->ui->leOutputROMLocation->text();
    this->pluginSettings->overwriteOuputROM = this->ui->cbOverwriteOutputROM->isChecked();
    this->pluginSettings->infiniteLives = this->ui->cbInfiniteLives->isChecked();
    this->pluginSettings->music = this->ui->comboMusic->currentIndex();
    this->pluginSettings->combineMusicWithOtherPacks = this->ui->cbCombineWithOtherMusicPacks->isChecked();
    this->pluginSettings->toneColor = this->ui->comboTone->currentIndex();
    this->pluginSettings->graphics = this->ui->comboGraphics->currentIndex();
    if (this->pluginSettings->infiniteLives) this->pluginSettings->numLives = 1;
    else this->pluginSettings->numLives = this->ui->sbLives->value();
    this->pluginSettings->godMode = this->ui->cbGodMode->isChecked();
    this->pluginSettings->addLuigiGame = !this->ui->cb2PlayerGame->isChecked();
    this->pluginSettings->superMarioOnDamage = this->ui->cbRevertToSuperMario->checkState();
    this->pluginSettings->lakituThrowArc = this->ui->cbLakituThrowArc->checkState();
    this->pluginSettings->autoscroll = this->ui->cbAutoscroll->checkState();
    this->pluginSettings->replaceFireFlowerWithHammerSuit = this->ui->cbReplaceFireFlowerWithHammerSuit->checkState();
    this->pluginSettings->enemySpeed = this->ui->comboEnemySpeed->currentIndex()+1;

    this->pluginSettings->generateNewLevels = this->ui->cbGenerateNewLevels->isChecked();
    if (!this->pluginSettings->generateNewLevels) {
        this->pluginSettings->levelScripts = this->ui->comboLevelScripts->currentText();
    } else {
        this->pluginSettings->randomNumWorlds = this->randomNumWorlds;
        if (!this->randomNumWorlds) {
            this->pluginSettings->numWorlds = this->ui->sbNumWorlds->value();
            this->pluginSettings->numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
        }
        this->pluginSettings->standardOverworldChance = this->ui->comboStandardOverworld->currentText();
        this->pluginSettings->undergroundChance = this->ui->comboUnderground->currentText();
        this->pluginSettings->underwaterChance = this->ui->comboUnderwater->currentText();
        this->pluginSettings->bridgeChance = this->ui->comboBridge->currentText();
        this->pluginSettings->islandChance = this->ui->comboIsland->currentText();
        this->pluginSettings->randomSeed = this->ui->sbRandomSeed->value();

        //Save the Difficulty Settings
        this->pluginSettings->difficultyComboIndex = this->ui->comboDifficulty->currentIndex();
        switch (this->pluginSettings->difficultyComboIndex) {
        default: assert(false); break;
        case 0: //Custom
            this->pluginSettings->difficultyBulletTime = this->difficultySettings.bulletTime;
            this->pluginSettings->difficultyHammerTime = this->difficultySettings.hammerTime;
            this->pluginSettings->difficultyBuzzyBeetlesReplaceLoneGoombas = this->difficultySettings.buzzyBeetlesReplaceLoneGoombas;
            this->pluginSettings->difficultyBridgeFlyingCheepCheeps = this->difficultySettings.bridgeFlyingCheepCheeps;
            this->pluginSettings->difficultyBridgeLakitus = this->difficultySettings.bridgeLakitus;
            this->pluginSettings->difficultyBridgeOffscreenBulletBills = this->difficultySettings.bridgeOffscreenBulletBills;
            this->pluginSettings->difficultyCastleFlyingCheepCheeps = this->difficultySettings.castleFlyingCheepCheeps;
            this->pluginSettings->difficultyCastleLakitus = this->difficultySettings.castleLakitus;
            this->pluginSettings->difficultyCastleOffscreenBulletBills = this->difficultySettings.castleOffscreenBulletBills;
            this->pluginSettings->difficultyIslandFlyingCheepCheeps = this->difficultySettings.islandFlyingCheepCheeps;
            this->pluginSettings->difficultyIslandLakitus = this->difficultySettings.islandLakitus;
            this->pluginSettings->difficultyIslandOffscreenBulletBills = this->difficultySettings.islandOffscreenBulletBills;
            this->pluginSettings->difficultyUndergroundFlyingCheepCheeps = this->difficultySettings.undergroundFlyingCheepCheeps;
            this->pluginSettings->difficultyUndergroundLakitus = this->difficultySettings.undergroundLakitus;
            this->pluginSettings->difficultyUndergroundOffscreenBulletBills = this->difficultySettings.undergroundOffscreenBulletBills;
            this->pluginSettings->difficultyUnderwaterBloopers = this->difficultySettings.underwaterBloopers;
            this->pluginSettings->difficultyUnderwaterFlyingCheepCheeps = this->difficultySettings.underwaterFlyingCheepCheeps;
            this->pluginSettings->difficultyUnderwaterLakitus = this->difficultySettings.underwaterLakitus;
            this->pluginSettings->difficultyUnderwaterSwimmingCheepCheeps = this->difficultySettings.underwaterSwimmingCheepCheeps;
            this->pluginSettings->difficultyStandardOverworldFlyingCheepCheeps = this->difficultySettings.standardOverworldFlyingCheepCheeps;
            this->pluginSettings->difficultyStandardOverworldLakitus = this->difficultySettings.standardOverworldLakitus;
            this->pluginSettings->difficultyStandardOverworldOffscreenBulletBills = this->difficultySettings.standardOverworldOffscreenBulletBills;
            this->pluginSettings->difficultyNoEnemies = this->difficultySettings.noEnemies;
            this->pluginSettings->difficultyHammerTimeIntensity = this->difficultySettings.hammerTimeIntensity;
            this->pluginSettings->difficultyLakituSpawnChancePerLevel = this->difficultySettings.lakituSpawnChancePerLevel;
            this->pluginSettings->difficultyDisableAllOtherEnemiesWhenALakituSpawns = this->difficultySettings.disableAllOtherEnemiesWhenALakituSpawns;
            this->pluginSettings->difficultySpawnerPriority = this->difficultySettings.spawnerPriority;
            break;
        case 1: //Random
            break; //do nothing for now
        case 2: //Very Easy
            Difficulty_Level_Configurations().Very_Easy(this->pluginSettings);
            break;
        case 3: //Easy
            Difficulty_Level_Configurations().Easy(this->pluginSettings);
            break;
        case 4: //Normal
            Difficulty_Level_Configurations().Normal(this->pluginSettings);
            break;
        case 5: //Hard
            Difficulty_Level_Configurations().Hard(this->pluginSettings);
            break;
        case 6: //Very Hard
            Difficulty_Level_Configurations().Very_Hard(this->pluginSettings);
            break;
        case 7: //Brutal
            Difficulty_Level_Configurations().Brutal(this->pluginSettings);
            break;
        case 8: //Walk in the Park
            Difficulty_Level_Configurations().Walk_In_The_Park(this->pluginSettings);
            break;
        case 9: //Riddled with Bullets
            Difficulty_Level_Configurations().Riddled_With_Bullets(this->pluginSettings);
            break;
        case 10: //Lakitu's Challenge
            Difficulty_Level_Configurations().Lakitus_Challenge(this->pluginSettings);
            break;
        case 11: //Cheep-Cheep Frenzy
            Difficulty_Level_Configurations().Cheep_Cheep_Frenzy(this->pluginSettings);
            break;
        case 12: //Hammer Time
            Difficulty_Level_Configurations().Hammer_Time(this->pluginSettings);
            break;
        case 13: //Extreme Hammer Time
            Difficulty_Level_Configurations().Extreme_Hammer_Time(this->pluginSettings);
            break;
        }
    }

    this->close();
}

void Configure_Settings_Form::on_btnInstallNewROM_clicked() {
    if (!this->writerPlugin->Install_ROM().isEmpty()) {
        this->Populate_Installed_ROMs();
    }
}

void Configure_Settings_Form::on_cbInfiniteLives_toggled(bool checked) {
    this->ui->lblLives->setEnabled(!checked);
    if (checked) this->ui->sbLives->clear();
    else this->ui->sbLives->setValue(this->pluginSettings->numLives);
    this->ui->sbLives->setEnabled(!checked);
}

void Configure_Settings_Form::on_btnOutputROMLocation_clicked() {
    QString extension = this->pluginSettings->baseROM.split('.').last().toLower();
    QString extensionFilter = "";
    if (extension == "fds") extensionFilter = "Famicom Images (*.fds)";
    else if (extension == "nes") extensionFilter = "NES ROMs (*.nes)";
    else extensionFilter = "NES ROMs (*.nes *.fds)";

    //Ask the user where they want to save the output ROM
    QString openLocation = QFileInfo(this->ui->leOutputROMLocation->text()).absolutePath();
    if (!QFileInfo(openLocation).exists()) openLocation = this->applicationLocation;
    QString outputROMLocation = QFileDialog::getSaveFileName(this->parent, "Save Location", openLocation, extensionFilter);
    if (outputROMLocation.isEmpty()) return;
    else this->ui->leOutputROMLocation->setText(outputROMLocation);
}

void Configure_Settings_Form::on_buttonBox_clicked(QAbstractButton *button) {
    assert(button);
    if (!this->ui->buttonBox->buttons().contains(button)) return; //ignore other buttons
    if (this->ui->buttonBox->buttonRole(button) != QDialogButtonBox::AcceptRole) {
        this->close();
        return;
    }
    if (!this->At_Least_One_Very_Common_Selected()) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "At least one level type must have a \"" + STRING_VERY_COMMON + "\" chance!",
                              Common_Strings::STRING_OK);
        return;
    }
    if (this->ui->comboLevelScripts->currentText() == STRING_NO_LEVEL_SCRIPTS_FOUND) {
        QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED,
                              "No level scripts could be found! Try generating new levels.",
                              Common_Strings::STRING_OK);
        return;
    }

    this->Save_Settings();
    this->close();
}

void Configure_Settings_Form::Clear_Chance_ComboBoxes() {
    this->ui->comboDifficulty->setCurrentIndex(0);
    this->ui->comboStandardOverworld->clear();
    this->ui->comboUnderground->clear();
    this->ui->comboUnderwater->clear();
    this->ui->comboBridge->clear();
    this->ui->comboIsland->clear();
}

void Configure_Settings_Form::Populate_Chance_ComboBoxes() {
    this->Populate_Chance_ComboBox(this->ui->comboStandardOverworld);
    this->Populate_Chance_ComboBox(this->ui->comboUnderground);
    this->Populate_Chance_ComboBox(this->ui->comboUnderwater);
    this->Populate_Chance_ComboBox(this->ui->comboBridge);
    this->Populate_Chance_ComboBox(this->ui->comboIsland);
    this->ui->comboDifficulty->setCurrentIndex(this->difficultyComboIndex);
    this->ui->comboStandardOverworld->setCurrentText(this->pluginSettings->standardOverworldChance);
    this->ui->comboUnderground->setCurrentText(this->pluginSettings->undergroundChance);
    this->ui->comboUnderwater->setCurrentText(this->pluginSettings->underwaterChance);
    this->ui->comboBridge->setCurrentText(this->pluginSettings->bridgeChance);
    this->ui->comboIsland->setCurrentText(this->pluginSettings->islandChance);
}

void Configure_Settings_Form::Populate_Chance_ComboBox(QComboBox *comboBox) {
    assert(comboBox);
    comboBox->clear();
    comboBox->addItem(STRING_VERY_COMMON);
    comboBox->addItem(STRING_COMMON);
    comboBox->addItem(STRING_UNCOMMON);
    comboBox->addItem(STRING_RARE);
    comboBox->addItem(STRING_NONE);
}

void Configure_Settings_Form::Populate_Level_Scripts_ComboBox() {
    this->ui->comboLevelScripts->clear();
    QDir dir(this->levelLocation);
    if (!dir.exists()) {
        this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
        return;
    }

    //Scan for valid level scripts
    QStringList levelFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
    QStringList validLevelFolders;
    QStringList validRandomFolders;
    foreach (QString level, levelFolders) {
        if (!dir.cd(level)) continue;
        if (dir.exists(level + ".map")) {
            if (level.startsWith("Random ")) validRandomFolders.append(level);
            else validLevelFolders.append(level);
        }
        if (!dir.cdUp()) {
            this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
            return; //this shouldn't happen unless the parent directory is removed
        }
    }

    //Add the valid folders to the ComboBox
    if (validLevelFolders.isEmpty() && validRandomFolders.isEmpty()) {
        this->ui->comboLevelScripts->addItem(STRING_NO_LEVEL_SCRIPTS_FOUND);
    } else {
        if (!validLevelFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validLevelFolders);
        if (!validRandomFolders.isEmpty()) this->ui->comboLevelScripts->addItems(validRandomFolders);
    }

    //Use the last selected level scripts
    if (validLevelFolders.contains(this->pluginSettings->levelScripts) || validRandomFolders.contains(this->pluginSettings->levelScripts)) {
        this->ui->comboLevelScripts->setCurrentText(this->pluginSettings->levelScripts);
    } else {
        this->ui->comboLevelScripts->setCurrentIndex(0);
    }
}

bool Configure_Settings_Form::At_Least_One_Very_Common_Selected() {
    if (!this->ui->cbGenerateNewLevels->isChecked()) return true;
    if (this->ui->comboStandardOverworld->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderground->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboUnderwater->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboBridge->currentText() == STRING_VERY_COMMON) return true;
    if (this->ui->comboIsland->currentText() == STRING_VERY_COMMON) return true;
    return false;
}

void Configure_Settings_Form::Enable_New_Level_Options(bool enable) {
    this->loading = true;

    //Toggle the Level Scripts
    this->ui->lblLevelScripts->setEnabled(!enable);
    this->ui->comboLevelScripts->setEnabled(!enable);
    this->ui->btnClearAllRandomLevelScripts->setEnabled(!enable);

    //Toggle the checkboxes
    this->ui->cbRandomNumWorlds->setEnabled(enable);
    if (enable) this->ui->cbRandomNumWorlds->setChecked(this->randomNumWorlds);
    else this->ui->cbRandomNumWorlds->setChecked(false);

    //Toggle the spinboxes
    this->ui->lblNumWorlds->setEnabled(enable && !this->randomNumWorlds);
    this->ui->sbNumWorlds->setEnabled(enable && !this->randomNumWorlds);
    if (enable && !this->randomNumWorlds) { //reset the values of the spinboxes
        this->ui->sbNumWorlds->setValue(this->pluginSettings->numWorlds);
        this->ui->sbNumLevelsPerWorld->setValue(this->pluginSettings->numLevelsPerWorld);
    } else {
        this->ui->sbNumWorlds->clear();
        this->ui->sbNumLevelsPerWorld->clear();
    }
    this->ui->lblNumLevelsPerWorld->setEnabled(enable && !this->randomNumWorlds);
    this->ui->sbNumLevelsPerWorld->setEnabled(enable && !this->randomNumWorlds);


    //Toggle the comboboxes
    this->ui->lblDifficulty->setEnabled(enable);
    this->ui->lblStandardOverworld->setEnabled(enable);
    this->ui->lblUnderground->setEnabled(enable);
    this->ui->lblUnderwater->setEnabled(enable);
    this->ui->lblBridge->setEnabled(enable);
    this->ui->lblIsland->setEnabled(enable);
    this->ui->comboDifficulty->setEnabled(enable);
    this->ui->comboStandardOverworld->setEnabled(enable);
    this->ui->comboUnderground->setEnabled(enable);
    this->ui->comboUnderwater->setEnabled(enable);
    this->ui->comboBridge->setEnabled(enable);
    this->ui->comboIsland->setEnabled(enable);

    //Toggle the random seed
    this->ui->lblRandomSeed->setEnabled(enable);
    this->ui->sbRandomSeed->setEnabled(enable);
    this->ui->btnNewRandomSeed->setEnabled(enable);
    if (enable) this->ui->sbRandomSeed->setValue(this->pluginSettings->randomSeed);
    else this->ui->sbRandomSeed->clear();

    //Populate the appropriate ComboBoxes
    if (enable) {
        this->ui->comboLevelScripts->clear();
        this->Populate_Chance_ComboBoxes();
    } else {
        this->Clear_Chance_ComboBoxes();
        this->Populate_Level_Scripts_ComboBox();
    }
    this->loading = false;
}

void Configure_Settings_Form::on_cbGenerateNewLevels_toggled(bool checked) {
    this->Enable_New_Level_Options(checked);
}

void Configure_Settings_Form::on_btnClearAllRandomLevelScripts_clicked() {
    QMessageBox::StandardButton answer;
    answer = QMessageBox::question(this, Common_Strings::STRING_LEVEL_HEADED,
                                   "This will delete all of your previously generated random level scripts! Are you sure that you want to do this?",
                                   QMessageBox::Yes | QMessageBox::No);

    //Delete all random level scripts
    if (answer == QMessageBox::Yes) {
        QDir dir(this->levelLocation);
        bool success = true;
        if (dir.exists()) {
            //Scan for valid level scripts
            QStringList levelFolders = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot, QDir::Name);
            foreach (QString level, levelFolders) {
                if (level.size() == 28 && level.startsWith("Random ")) {
                    QDir levelDir(dir.path() + "/" + level);
                    if (!levelDir.removeRecursively()) success = false;
                }
            }
        }

        //Show a message for the status of the operation
        if (success) {
            QMessageBox::information(this, Common_Strings::STRING_LEVEL_HEADED,
                                     "All previously generated random level scripts have been removed!",
                                     Common_Strings::STRING_OK);
        } else {
            QMessageBox::critical(this, Common_Strings::STRING_LEVEL_HEADED, Common_Strings::STRING_LEVEL_HEADED +
                                  " could not delete all of the randomly generated level scripts!",
                                  Common_Strings::STRING_OK);
        }
        this->Populate_Level_Scripts_ComboBox();
        this->Save_Settings();
    }
}

void Configure_Settings_Form::on_cbRandomNumWorlds_clicked(bool checked) {
    this->ui->lblNumWorlds->setEnabled(!checked);
    this->ui->sbNumWorlds->setEnabled(!checked);
    this->ui->lblNumLevelsPerWorld->setEnabled(!checked);
    this->ui->sbNumLevelsPerWorld->setEnabled(!checked);
    if (checked) {
        this->ui->sbNumWorlds->clear();
        this->ui->sbNumLevelsPerWorld->clear();
    } else {
        this->ui->sbNumWorlds->setValue(this->numWorlds);
        this->ui->sbNumLevelsPerWorld->setValue(this->numLevelsPerWorld);
    }
    this->randomNumWorlds = checked;
}

void Configure_Settings_Form::on_sbNumLevelsPerWorld_valueChanged(int numLevelsPerWorld) {
    int numWorlds = this->ui->sbNumWorlds->value();
    while (numLevelsPerWorld*numWorlds > 20) {
        --numWorlds;
    }
    this->ui->sbNumWorlds->setValue(numWorlds);
    this->numLevelsPerWorld = numLevelsPerWorld;
}

void Configure_Settings_Form::on_sbNumWorlds_valueChanged(int numWorlds) {
    int numLevelsPerWorld = this->ui->sbNumLevelsPerWorld->value();
    while (numLevelsPerWorld*numWorlds > 20) {
        --numLevelsPerWorld;
    }
    this->ui->sbNumLevelsPerWorld->setValue(numLevelsPerWorld);
    this->numWorlds = numWorlds;
}

void Configure_Settings_Form::on_btnNewRandomSeed_clicked() {
    this->ui->sbRandomSeed->setValue(QTime::currentTime().msecsSinceStartOfDay());
}

void Configure_Settings_Form::on_comboDifficulty_activated(int index) {
    if (this->loading) return;
    if (index == 0) { //Custom
        //Show the Difficulty Form
        Difficulty_Level_Form form(this, &this->difficultySettings, this->applicationLocation);
        form.exec();
        if (!this->difficultySettings.save) {
            this->ui->comboDifficulty->setCurrentIndex(this->difficultyComboIndex);
            return;
        }
    }
    this->difficultyComboIndex = index;
}
