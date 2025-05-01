# Custom Text Editor with Vim Motions 📝

Welcome to my custom text editor project! 🎉  

## *About*  
This is a personal project where I am building a text editor using C++ and SDL. The editor includes Vim-like motions and features, providing a customizable experience for editing text. Through this project, I aim to:  
- 🖥️ Develop a text editor that supports basic text editing functions.  
- ⌨️ Integrate Vim-style motions for efficient navigation.  
- 💻 Enhance my understanding of C++ programming and SDL.

## Goals 🎯  
- 🏗️ Build a fully functional text editor.  
- 🧑‍💻 Implement Vim-style motions, such as navigation using `h`, `j`, `k`, `l`, and other Vim commands.  
- 📚 Gain experience with C++ and SDL for GUI development.  
- 🗂️ Support additional features like syntax highlighting, error display, and multiple file support.

## Features ✨  
- **Vim-like Motion Support**: Navigate text like you would in Vim using `h`, `j`, `k`, `l`, and other Vim keys.  
- ✏️ **Text Editing**: Add, remove, and modify text in a flexible, user-friendly environment.  
- 🎨 **Syntax Highlighting**: Highlight different programming languages with color coding for better readability.  
- 📝 **Multi-file Support**: Open, edit, and save multiple files at once.  
- ⚠️ **Error Display**: Show compilation errors in an integrated errors area to help with debugging.

## *Text Editor Overview* 💡

### 1. Window and Layout 🖼️  
The editor’s window is created using SDL, with support for different platforms (Windows, Linux, macOS). The interface is divided into distinct areas:  

- **Text Area**: Where the content is displayed and edited. This area allows you to type, modify, and navigate your text.
  
- **Command Line Area**: For Vim-style commands and input. Here, you can perform actions like saving (`:w`), quitting (`:q`), and other text editing operations.

- **File Area**: To manage and switch between open files. You can easily open new files and navigate through them in this area.

- **Errors Area**: For displaying compiler errors when using a C++ file. This area shows you detailed error messages and allows you to navigate directly to the lines with issues.

### 2. Vim Motions and Commands ⌨️  
The text editor supports common Vim motions, such as:  
- 🔽 **Arrow keys**: Move the cursor up, down, left, or right.  
- 🧭 **`h`, `j`, `k`, `l`**: Navigate the text (left, down, up, right).  
- 📜 **$**: Move to the beginning of the file.  

The editor also supports more advanced Vim commands like `:w` for saving, `:q` for quitting, and more.

### 3. Multi-file Support 🗂️  
You can open, close, and switch between multiple files. The File Area shows all open files and allows you to quickly jump between them without losing your place.

### 4. Cross-Platform Support 🌐  
The editor is designed to work on multiple platforms, including Windows, Linux, and macOS, using SDL for rendering. 

### 5. Command Line Area 🖥️  
The Command Line Area is integrated with Vim commands, allowing you to perform actions such as saving files, opening new files, and searching through the document with a few keystrokes.

---

Feel free to explore the code, contribute, or fork the project to add more features! Enjoy using the editor! 😄

---

## *Current Focus* 🔧
- **Error Support**: Implementing detailed error highlighting for better debugging.
- **Expanding Vim Motions**: Adding more Vim-like features for smoother text navigation and editing.
- **UI Clean-Up**: Refactoring the user interface to be more user-friendly and intuitive.
- **Color Enhancements**: Making better use of colors to highlight errors, syntax, and commands for a visually appealing experience.

---

![image](https://github.com/user-attachments/assets/0bb25e8c-c088-4e8e-928b-0dfd30f7b30a)

