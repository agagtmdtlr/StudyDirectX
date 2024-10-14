#pragma once

#include "Controller.h"
#include <algorithm>
#include <array>
#include <cctype>
#include <cstring>
#include <filesystem>
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include "Texture.h"
using ImGuiFileBrowserFlags = int;

enum ImGuiFileBrowserFlags_ : std::uint32_t
{
    ImGuiFileBrowserFlags_SelectDirectory = 1 << 0,  // select directory instead of regular file
    ImGuiFileBrowserFlags_EnterNewFilename = 1 << 1,  // allow user to enter new filename when selecting regular file
    ImGuiFileBrowserFlags_NoModal = 1 << 2,  // file browsing window is modal by default. specify this to use a popup window
    ImGuiFileBrowserFlags_NoTitleBar = 1 << 3,  // hide window title bar
    ImGuiFileBrowserFlags_NoStatusBar = 1 << 4,  // hide status bar at the bottom of browsing window
    ImGuiFileBrowserFlags_CloseOnEsc = 1 << 5,  // close file browser when pressing 'ESC'
    ImGuiFileBrowserFlags_CreateNewDir = 1 << 6,  // allow user to create new directory
    ImGuiFileBrowserFlags_MultipleSelection = 1 << 7,  // allow user to select multiple files. this will hide ImGuiFileBrowserFlags_EnterNewFilename
    ImGuiFileBrowserFlags_HideRegularFiles = 1 << 8,  // hide regular files when ImGuiFileBrowserFlags_SelectDirectory is enabled
    ImGuiFileBrowserFlags_ConfirmOnEnter = 1 << 9,  // confirm selection when pressing 'ENTER'
    ImGuiFileBrowserFlags_SkipItemsCausingError = 1 << 10, // when entering a new directory, any error will interrupt the process, causing the file browser to fall back to the working directory.
    // with this flag, if an error is caused by a specific item in the directory, that item will be skipped, allowing the process to continue.
    ImGuiFileBrowserFlags_EditPathString = 1 << 11, // allow user to directly edit the whole path string
};

class FileBrowser : public Controller
{

public:
    FileBrowser();
    virtual void Render();

    void Move(std::filesystem::path pth );
    std::filesystem::path curpath;
    std::filesystem::path rootPath;

    std::vector<std::filesystem::directory_entry> entrys;
    std::vector<std::string> tokens;

    shared_ptr<Texture> folderIco;
    shared_ptr<Texture> fileIco;
    shared_ptr<Texture> imgIco;

};