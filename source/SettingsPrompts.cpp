#include <gccore.h>
#include <unistd.h>
#include <string.h>

#include "filelist.h"
#include "sys.h"
#include "language/language.h"
#include "libwiigui/gui.h"
#include "libwiigui/gui_customoptionbrowser.h"
#include "listfiles.h"
#include "cfg.h"
#include "main.h"
#include "PromptWindows.h"
#include "fatmounter.h"


/*** Extern variables ***/
extern GuiWindow * mainWindow;
extern GuiSound * bgMusic;
extern u8 shutdown;
extern u8 reset;

/*** Extern functions ***/
extern void ResumeGui();
extern void HaltGui();


/****************************************************************************
 * MenuOGG
 ***************************************************************************/
bool MenuOGG()
{
    int cnt = 0;
    int ret = 0, choice = 0;
    int scrollon, nothingchanged = 0;
    bool returnhere = false;

	GuiSound btnSoundOver(button_over_pcm, button_over_pcm_size, SOUND_PCM, Settings.sfxvolume);
	GuiSound btnClick(button_click2_pcm, button_click2_pcm_size, SOUND_PCM, Settings.sfxvolume);

	char imgPath[100];

	snprintf(imgPath, sizeof(imgPath), "%sbutton_dialogue_box.png", CFG.theme_path);
	GuiImageData btnOutline(imgPath, button_dialogue_box_png);
	snprintf(imgPath, sizeof(imgPath), "%ssettings_background.png", CFG.theme_path);
	GuiImageData settingsbg(imgPath, settings_background_png);

    GuiTrigger trigA;
	trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	GuiTrigger trigB;
	trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);
    GuiTrigger trigMinus;
	trigMinus.SetButtonOnlyTrigger(-1, WPAD_BUTTON_MINUS | WPAD_CLASSIC_BUTTON_MINUS, 0);
	GuiTrigger trigPlus;
	trigPlus.SetButtonOnlyTrigger(-1, WPAD_BUTTON_PLUS | WPAD_CLASSIC_BUTTON_PLUS, 0);

    char fullpath[150];
    char shortpath[35];
	int countoggs = GetAllDirFiles(Settings.oggload_path);

    if(!strcmp("", Settings.oggload_path)) {
        sprintf(shortpath, "%s", LANGUAGE.Standard);
	} else {
		sprintf(shortpath, "%s", Settings.oggload_path);
	}

    GuiText titleTxt(shortpath, 24, (GXColor){0, 0, 0, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	titleTxt.SetPosition(0,0);
	GuiButton pathBtn(300, 50);
	pathBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	pathBtn.SetPosition(0,28);
	pathBtn.SetLabel(&titleTxt);
	pathBtn.SetSoundOver(&btnSoundOver);
	pathBtn.SetSoundClick(&btnClick);
	pathBtn.SetTrigger(&trigA);
	pathBtn.SetEffectGrow();

    GuiImage oggmenubackground(&settingsbg);
	oggmenubackground.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	oggmenubackground.SetPosition(0, 0);

    GuiText backBtnTxt(LANGUAGE.Back , 22, (GXColor){THEME.prompttxt_r, THEME.prompttxt_g, THEME.prompttxt_b, 255});
	backBtnTxt.SetMaxWidth(btnOutline.GetWidth()-30);
	GuiImage backBtnImg(&btnOutline);
	if (Settings.wsprompt == yes){
	backBtnTxt.SetWidescreen(CFG.widescreen);
	backBtnImg.SetWidescreen(CFG.widescreen);
	}
	GuiButton backBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	backBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtn.SetPosition(-180, 400);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetSoundOver(&btnSoundOver);
	backBtn.SetSoundClick(&btnClick);
	backBtn.SetTrigger(&trigA);
	backBtn.SetTrigger(&trigB);
	backBtn.SetEffectGrow();

	GuiText defaultBtnTxt(LANGUAGE.Default , 22, (GXColor){THEME.prompttxt_r, THEME.prompttxt_g, THEME.prompttxt_b, 255});
	defaultBtnTxt.SetMaxWidth(btnOutline.GetWidth()-30);
	GuiImage defaultBtnImg(&btnOutline);
	if (Settings.wsprompt == yes){
	defaultBtnTxt.SetWidescreen(CFG.widescreen);
	defaultBtnImg.SetWidescreen(CFG.widescreen);
	}
	GuiButton defaultBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	defaultBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	defaultBtn.SetPosition(180, 400);
	defaultBtn.SetLabel(&defaultBtnTxt);
	defaultBtn.SetImage(&defaultBtnImg);
	defaultBtn.SetSoundOver(&btnSoundOver);
	defaultBtn.SetSoundClick(&btnClick);
	defaultBtn.SetTrigger(&trigA);
	defaultBtn.SetEffectGrow();

    customOptionList options2(countoggs);

    for (cnt = 0; cnt < countoggs; cnt++) {
        options2.SetValue(cnt, "%s", GetFileName(cnt));
        options2.SetName(cnt,"%i.", cnt+1);
    }

	if(cnt < 9) {
    scrollon = 0;
    } else {
    scrollon = 1;
    }

	GuiCustomOptionBrowser optionBrowser4(396, 280, &options2, CFG.theme_path, "bg_options_settings.png", bg_options_settings_png, scrollon, 10);
	optionBrowser4.SetPosition(0, 90);
	optionBrowser4.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);

	snprintf(imgPath, sizeof(imgPath), "%smp3_stop.png", CFG.theme_path);
	GuiImageData stop(imgPath, mp3_stop_png);
	snprintf(imgPath, sizeof(imgPath), "%sstartgame_arrow_right.png", CFG.theme_path);
	GuiImageData play(imgPath, startgame_arrow_right_png);

	GuiImage playBtnImg(&play);
	playBtnImg.SetWidescreen(CFG.widescreen);
	GuiButton playBtn(play.GetWidth(), play.GetHeight());
	playBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	playBtn.SetPosition(50, 400);
	playBtn.SetImage(&playBtnImg);
	playBtn.SetSoundOver(&btnSoundOver);
	playBtn.SetSoundClick(&btnClick);
	playBtn.SetTrigger(&trigA);
	playBtn.SetTrigger(&trigPlus);
	playBtn.SetEffectGrow();

	GuiImage stopBtnImg(&stop);
	stopBtnImg.SetWidescreen(CFG.widescreen);
	GuiButton stopBtn(stop.GetWidth(), stop.GetHeight());
	stopBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	stopBtn.SetPosition(-15, 400);
	stopBtn.SetImage(&stopBtnImg);
	stopBtn.SetSoundOver(&btnSoundOver);
	stopBtn.SetSoundClick(&btnClick);
	stopBtn.SetTrigger(&trigA);
	stopBtn.SetTrigger(&trigMinus);
	stopBtn.SetEffectGrow();

    HaltGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&oggmenubackground);
	w.Append(&pathBtn);
    w.Append(&backBtn);
    w.Append(&playBtn);
	w.Append(&stopBtn);
	w.Append(&defaultBtn);
    w.Append(&optionBrowser4);
    mainWindow->Append(&w);

	w.SetEffect(EFFECT_FADE, 20);
	ResumeGui();

	while(w.GetEffect()>0) usleep(50);

	while(!returnhere)
	{

    if(shutdown == 1)
		Sys_Shutdown();
	if(reset == 1)
        Sys_Reboot();

    if (backBtn.GetState() == STATE_CLICKED) {
            if(nothingchanged == 1 && countoggs > 0) {
            if(!strcmp("", Settings.oggload_path) || !strcmp("notset", Settings.ogg_path)) {
                bgMusic->Play();
            } else {
                bgMusic->PlayOggFile(Settings.ogg_path);
            }
            }
            backBtn.ResetState();
			break;
    }

    if (defaultBtn.GetState() == STATE_CLICKED) {
            choice = WindowPrompt(LANGUAGE.Loadingstandardmusic,0,LANGUAGE.ok, LANGUAGE.Cancel,0,0);
            if (choice == 1) {
            sprintf(Settings.ogg_path, "notset");
            bgMusic->Play();
            SetVolumeOgg(255*(Settings.volume/100.0));
            cfg_save_global();
            }
            defaultBtn.ResetState();
			optionBrowser4.SetFocus(1);
    }

    if (pathBtn.GetState() == STATE_CLICKED) {
            w.Remove(&optionBrowser4);
            w.Remove(&backBtn);
            w.Remove(&pathBtn);
            w.Remove(&playBtn);
            w.Remove(&stopBtn);
			w.Remove(&defaultBtn);
            char entered[43] = "";
            strncpy(entered, Settings.oggload_path, sizeof(entered));
            int result = OnScreenKeyboard(entered,43,0);
            w.Append(&optionBrowser4);
            w.Append(&pathBtn);
            w.Append(&backBtn);
            w.Append(&playBtn);
            w.Append(&stopBtn);
			w.Append(&defaultBtn);
            if ( result == 1 ) {
                int len = (strlen(entered)-1);
                if(entered[len] !='/')
                strncat (entered, "/", 1);
                strncpy(Settings.oggload_path, entered, sizeof(Settings.oggload_path));
                WindowPrompt(LANGUAGE.Backgroundmusicpath,0,LANGUAGE.ok,0,0,0);
//                if(isSdInserted()) {
				if(isInserted(bootDevice)) {
                    if(!strcmp("", Settings.oggload_path)) {
                    sprintf(Settings.ogg_path, "notset");
                    bgMusic->Play();
                    }
                    cfg_save_global();
                    returnhere = true;
                    break;
                } else {
                    WindowPrompt(LANGUAGE.NoSDcardinserted, LANGUAGE.InsertaSDCardtosave, LANGUAGE.ok, 0,0,0);
                }
            }
        if(countoggs > 0) {
            optionBrowser4.SetFocus(1);
        }
        pathBtn.ResetState();
    }

    ret = optionBrowser4.GetClickedOption();

    if(ret>=0) {
        choice = WindowPrompt(LANGUAGE.Setasbackgroundmusic,GetFileName(ret),LANGUAGE.Yes,LANGUAGE.No,0,0);
        if(choice == 1) {
        StopOgg();
        snprintf(fullpath,150,"%s%s",Settings.oggload_path,GetFileName(ret));
        choice = bgMusic->PlayOggFile(fullpath);
        if(choice < 0) {
        WindowPrompt(LANGUAGE.Notasupportedformat, LANGUAGE.Loadingstandardmusic, LANGUAGE.ok, 0,0,0);
        sprintf(Settings.ogg_path, "notset");
        bgMusic->Play();
        SetVolumeOgg(255*(Settings.volume/100.0));
        } else {
        snprintf(Settings.ogg_path, sizeof(Settings.ogg_path), "%s", fullpath);
        cfg_save_global();
        SetVolumeOgg(255*(Settings.volume/100.0));
        nothingchanged = 0;
        }
        }
        optionBrowser4.SetFocus(1);
    }

    if (playBtn.GetState() == STATE_CLICKED && countoggs > 0) {
         if(countoggs > 0) {
            ret = optionBrowser4.GetSelectedOption();
            snprintf(fullpath, 150,"%s%s", Settings.oggload_path,GetFileName(ret));
            choice = bgMusic->PlayOggFile(fullpath);
            if(choice < 0) {
            WindowPrompt(LANGUAGE.Notasupportedformat, LANGUAGE.Loadingstandardmusic, LANGUAGE.ok, 0,0,0);
            if(!strcmp("", Settings.oggload_path) || !strcmp("notset", Settings.ogg_path)) {
                bgMusic->Play();
            } else {
                bgMusic->PlayOggFile(Settings.ogg_path);
            }
            }
            SetVolumeOgg(255*(Settings.volume/100.0));
			nothingchanged = 1;
            optionBrowser4.SetFocus(1);
         }
    playBtn.ResetState();
    }

	if(stopBtn.GetState() == STATE_CLICKED) {
	    if(countoggs > 0) {
            StopOgg();
            nothingchanged = 1;
            optionBrowser4.SetFocus(1);
	    }
        stopBtn.ResetState();
    }
	}

	w.SetEffect(EFFECT_FADE, -20);
	while(w.GetEffect()>0) usleep(50);

	HaltGui();
	mainWindow->Remove(&w);
	ResumeGui();

    return returnhere;
}

/****************************************************************************
 * MenuLanguageSelect
 ***************************************************************************/
int MenuLanguageSelect()
{
    int cnt = 0;
    int ret = 0, choice = 0;
    int scrollon;
    int returnhere = 0;

	GuiSound btnSoundOver(button_over_pcm, button_over_pcm_size, SOUND_PCM, Settings.sfxvolume);
	GuiSound btnClick(button_click2_pcm, button_click2_pcm_size, SOUND_PCM, Settings.sfxvolume);

	char imgPath[100];

	snprintf(imgPath, sizeof(imgPath), "%sbutton_dialogue_box.png", CFG.theme_path);
	GuiImageData btnOutline(imgPath, button_dialogue_box_png);
	snprintf(imgPath, sizeof(imgPath), "%ssettings_background.png", CFG.theme_path);
	GuiImageData settingsbg(imgPath, settings_background_png);

    GuiTrigger trigA;
	trigA.SetSimpleTrigger(-1, WPAD_BUTTON_A | WPAD_CLASSIC_BUTTON_A, PAD_BUTTON_A);
	GuiTrigger trigB;
	trigB.SetButtonOnlyTrigger(-1, WPAD_BUTTON_B | WPAD_CLASSIC_BUTTON_B, PAD_BUTTON_B);

    char fullpath[100];
	int countfiles = GetAllDirFiles(Settings.languagefiles_path);

    if(!strcmp("", Settings.languagefiles_path)) {
        sprintf(fullpath, "%s", LANGUAGE.Standard);
	} else {
		sprintf(fullpath, "%s", Settings.languagefiles_path);
	}

    GuiText titleTxt(fullpath, 24, (GXColor){0, 0, 0, 255});
	titleTxt.SetAlignment(ALIGN_CENTRE, ALIGN_MIDDLE);
	titleTxt.SetPosition(0,0);
	GuiButton pathBtn(300, 50);
	pathBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	pathBtn.SetPosition(0,28);
	pathBtn.SetLabel(&titleTxt);
	pathBtn.SetSoundOver(&btnSoundOver);
	pathBtn.SetSoundClick(&btnClick);
	pathBtn.SetTrigger(&trigA);
	pathBtn.SetEffectGrow();

    GuiImage oggmenubackground(&settingsbg);
	oggmenubackground.SetAlignment(ALIGN_LEFT, ALIGN_TOP);
	oggmenubackground.SetPosition(0, 0);

    GuiText backBtnTxt(LANGUAGE.Back , 22, (GXColor){THEME.prompttxt_r, THEME.prompttxt_g, THEME.prompttxt_b, 255});
	backBtnTxt.SetMaxWidth(btnOutline.GetWidth()-30);
	GuiImage backBtnImg(&btnOutline);
	if (Settings.wsprompt == yes){
	backBtnTxt.SetWidescreen(CFG.widescreen);
	backBtnImg.SetWidescreen(CFG.widescreen);
	}
	GuiButton backBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	backBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	backBtn.SetPosition(-180, 400);
	backBtn.SetLabel(&backBtnTxt);
	backBtn.SetImage(&backBtnImg);
	backBtn.SetSoundOver(&btnSoundOver);
	backBtn.SetSoundClick(&btnClick);
	backBtn.SetTrigger(&trigA);
	backBtn.SetTrigger(&trigB);
	backBtn.SetEffectGrow();

	GuiText defaultBtnTxt(LANGUAGE.Default , 22, (GXColor){THEME.prompttxt_r, THEME.prompttxt_g, THEME.prompttxt_b, 255});
	defaultBtnTxt.SetMaxWidth(btnOutline.GetWidth()-30);
	GuiImage defaultBtnImg(&btnOutline);
	if (Settings.wsprompt == yes) {
	defaultBtnTxt.SetWidescreen(CFG.widescreen);
	defaultBtnImg.SetWidescreen(CFG.widescreen);
	}
	GuiButton defaultBtn(btnOutline.GetWidth(), btnOutline.GetHeight());
	defaultBtn.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);
	defaultBtn.SetPosition(180, 400);
	defaultBtn.SetLabel(&defaultBtnTxt);
	defaultBtn.SetImage(&defaultBtnImg);
	defaultBtn.SetSoundOver(&btnSoundOver);
	defaultBtn.SetSoundClick(&btnClick);
	defaultBtn.SetTrigger(&trigA);
	defaultBtn.SetEffectGrow();

    customOptionList options2(countfiles);

    for (cnt = 0; cnt < countfiles; cnt++) {
        options2.SetName(cnt, "%s", GetFileName(cnt));
        options2.SetValue(cnt, NULL);
    }

	if(cnt < 9) {
    scrollon = 0;
    } else {
    scrollon = 1;
    }

	GuiCustomOptionBrowser optionBrowser4(396, 280, &options2, CFG.theme_path, "bg_options_settings.png", bg_options_settings_png, scrollon, 10);
	optionBrowser4.SetPosition(0, 90);
	optionBrowser4.SetAlignment(ALIGN_CENTRE, ALIGN_TOP);

    HaltGui();
	GuiWindow w(screenwidth, screenheight);
	w.Append(&oggmenubackground);
	w.Append(&pathBtn);
    w.Append(&backBtn);
	w.Append(&defaultBtn);
    w.Append(&optionBrowser4);
    mainWindow->Append(&w);

	w.SetEffect(EFFECT_FADE, 20);
	ResumeGui();

	while(w.GetEffect()>0) usleep(50);

	while(!returnhere)
	{

    if(shutdown == 1)
		Sys_Shutdown();
	if(reset == 1)
        Sys_Reboot();

    if (backBtn.GetState() == STATE_CLICKED) {

        backBtn.ResetState();
        break;
    }

    if (defaultBtn.GetState() == STATE_CLICKED) {
            choice = WindowPrompt(LANGUAGE.Loadingstandardlanguage,0,LANGUAGE.ok, LANGUAGE.Cancel,0,0);
            if (choice == 1) {
            sprintf(Settings.language_path, "notset");
            cfg_save_global();
            lang_default();
            CFG_Load();
            returnhere = 2;
            }
            defaultBtn.ResetState();
			optionBrowser4.SetFocus(1);
    }

    if (pathBtn.GetState() == STATE_CLICKED) {
            w.Remove(&optionBrowser4);
            w.Remove(&backBtn);
            w.Remove(&pathBtn);
			w.Remove(&defaultBtn);
            char entered[43] = "";
            strncpy(entered, Settings.languagefiles_path, sizeof(entered));
            int result = OnScreenKeyboard(entered,43,0);
            w.Append(&optionBrowser4);
            w.Append(&pathBtn);
            w.Append(&backBtn);
			w.Append(&defaultBtn);
            if ( result == 1 ) {
                int len = (strlen(entered)-1);
                if(entered[len] !='/')
                strncat (entered, "/", 1);
                strncpy(Settings.languagefiles_path, entered, sizeof(Settings.languagefiles_path));
                WindowPrompt(LANGUAGE.Languagepathchanged,0,LANGUAGE.ok,0,0,0);
//                if(isSdInserted()) {
				if(isInserted(bootDevice)) {
                    cfg_save_global();
                    returnhere = 1;
                    break;
                } else {
                    WindowPrompt(LANGUAGE.NoSDcardinserted, LANGUAGE.InsertaSDCardtosave, LANGUAGE.ok, 0,0,0);
                }
            }
        if(countfiles > 0) {
            optionBrowser4.SetFocus(1);
        }
        pathBtn.ResetState();
    }

    ret = optionBrowser4.GetClickedOption();

    if(ret>=0) {
        choice = WindowPrompt(LANGUAGE.Doyouwanttochangelanguage, 0, LANGUAGE.Yes, LANGUAGE.Cancel,0,0);
        if(choice == 1) {
//        if(isSdInserted()) {
		if(isInserted(bootDevice)) {
            snprintf(Settings.language_path, sizeof(Settings.language_path), "%s%s", Settings.languagefiles_path, GetFileName(ret));
            cfg_save_global();
            if(!checkfile(Settings.language_path)) {
                sprintf(Settings.language_path, "notset");
                WindowPrompt(LANGUAGE.Filenotfound,LANGUAGE.Loadingstandardlanguage,LANGUAGE.ok,0,0,0);
            }
            lang_default();
            CFG_Load();
            returnhere = 2;
            break;
        } else {
            WindowPrompt(LANGUAGE.NoSDcardinserted, LANGUAGE.InsertaSDCardtosave, LANGUAGE.ok, 0,0,0);
        }
        }
        optionBrowser4.SetFocus(1);
    }

	}

	w.SetEffect(EFFECT_FADE, -20);
	while(w.GetEffect()>0) usleep(50);

	HaltGui();
	mainWindow->Remove(&w);
	ResumeGui();

    return returnhere;
}

