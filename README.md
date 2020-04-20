# Разработка технологии для работы с базами данных с возможностью персонализации и пользовательского отображения результатов работы

Цель: создать инструмент для работы с базами данных.

Инструменты: Qt 5.14.2.

Особенности:

- возможность подключаться к различным базам данных (SQLite, Microsoft SQL, MySQL, PostgreSQL)
- персонализация внешнего вида отображаемых таблиц

---

[Задачи](Untitled/Untitled.csv)

[mikhailKhmel/database_viewer](http://github.com/mikhailKhmel/database_viewer)

---

Общая иерархия окон:

- [userdialog (окно выбора профиля)](https://github.com/mikhailKhmel/database_viewer/blob/master/userdialog.cpp)
    - [newprofile (создание нового профиля)](https://github.com/mikhailKhmel/database_viewer/blob/master/newprofile.cpp)
    - [mainwindow (основное окно)](https://github.com/mikhailKhmel/database_viewer/blob/master/mainwindow.cpp)
        - [connect_db (окно подключения к базе данных)](https://github.com/mikhailKhmel/database_viewer/blob/master/connect_db.cpp)
        - [create_table (окно создания новой таблицы)](https://github.com/mikhailKhmel/database_viewer/blob/master/create_table.cpp)
            - [create_column (окно создания нового поля)](https://github.com/mikhailKhmel/database_viewer/blob/master/create_column.cpp)
        - [create_column (окно создания нового поля)](https://github.com/mikhailKhmel/database_viewer/blob/master/create_column.cpp)
        - [delete_column (окно удаления поля в текущей таблице)](https://github.com/mikhailKhmel/database_viewer/blob/master/delete_column.cpp)
        - [rename_column (окно переименования поля в текущей таблице)](https://github.com/mikhailKhmel/database_viewer/blob/master/rename_column.cpp)
        - [uncover_column (раскрытия поля в текущей таблице)](https://github.com/mikhailKhmel/database_viewer/blob/master/uncover_columns.cpp)
        - [select_window (установка фильтра на поле в текущей таблице)](https://github.com/mikhailKhmel/database_viewer/blob/master/select_window.cpp)

---

- [config (файл конфигурации для хранения общих настроек для всего приложения)](https://github.com/mikhailKhmel/database_viewer/blob/master/config.cpp)
- [style.css (файл стилей приложения)](https://github.com/mikhailKhmel/database_viewer/blob/master/style.css)
