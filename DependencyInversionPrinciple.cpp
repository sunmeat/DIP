#include <iostream>
#include <string>
#include <vector>
using namespace std;

// Абстрактный класс для работы со статьями
class ArticleService {
public:
    virtual void createArticle(string title, string content) = 0;
    virtual void editArticle(string title, string content) = 0;
    virtual void deleteArticle(string title) = 0;
    // virtual void viewArticle(string title) = 0;
};

// Интерфейс для доступа к данным о статьях
__interface ArticleRepository {
    void saveArticle(string title, string content);
    void updateArticle(string title, string content);
    void deleteArticle(string title);
    vector<string> getTitles();
    string getContent(string title);
};

// Реализация ArticleRepository для хранения данных в файле
class InFileArticleRepository : public ArticleRepository {
private:
    vector<string> titles;
    vector<string> contents;
public:
    void saveArticle(string title, string content) {
        // сохранение статьи в файле
    }
    void updateArticle(string title, string content) {
        // обновление статьи в файле
    }
    void deleteArticle(string title) {
        // удаление статьи из файла
    }
    vector<string> getTitles() {
        return titles; // получение заголовков статей из файла
    }
    string getContent(string title) {
        return ""; // получение контента статьи по известному заголовку
    }
};

// Реализация ArticleRepository для хранения данных в базе данных
class DatabaseArticleRepository : public ArticleRepository {
private:
    vector<string> titles;
    vector<string> contents;
public:
    void saveArticle(string title, string content) {
        titles.push_back(title);
        contents.push_back(content);
    }
    void updateArticle(string title, string content) {
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == title) {
                contents[i] = content;
                break;
            }
        }
    }
    void deleteArticle(string title) {
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == title) {
                titles.erase(titles.begin() + i);
                contents.erase(contents.begin() + i);
                break;
            }
        }
    }
    vector<string> getTitles() {
        return titles;
    }
    string getContent(string title) {
        for (int i = 0; i < titles.size(); i++) {
            if (titles[i] == title) {
                return contents[i];
            }
        }
        throw "No article";
    }
};

// Реализация конкретного класса ArticleService для работы со статьями, хранящимися в БД
class DatabaseArticleService : public ArticleService {
private:
    ArticleRepository* repository;
public:
    DatabaseArticleService(ArticleRepository* repository) {
        this->repository = repository;
    }
    void createArticle(string title, string content) {
        repository->saveArticle(title, content);
    }
    void editArticle(string title, string content) {
        repository->updateArticle(title, content);
    }
    void deleteArticle(string title) {
        repository->deleteArticle(title);
    }
    vector<string> getTitles() {
        // Реализация получения заголовков статей
        return repository->getTitles();
    }
    string getContent(string title) {
        // Реализация получения содержимого статьи
        return repository->getContent(title);
    }
};

int main()
{
    setlocale(0, "");

    // Создание сервиса со статьями, хранящимися в БД
    DatabaseArticleRepository repository;
    DatabaseArticleService service(&repository);

    // Создание нескольких статей
    service.createArticle("Стаття 1", "Україна є суверенна i незалежна, демократична, соцiальна, правова держава.");
    service.createArticle("Стаття 2", "Суверенiтет України поширюється на всю її територiю.");
    service.createArticle("Стаття 3", "Людина, її життя i здоров'я, честь i гiднiсть, недоторканнiсть i безпека визнаються в Українi найвищою соцiальною цiннiстю.");

    // Получение заголовков всех статей
    vector<string> titles = service.getTitles();
    for (string title : titles) {
        cout << title << "\n";
        cout << service.getContent(title) << "\n\n";
    }

}

/*
Этот код демонстрирует пример использования шаблона
проектирования "Repository" для управления данными в различных
источниках хранения. Класс ArticleService представляет
абстрактный сервис для работы со статьями, который объявляет
методы для создания, редактирования и удаления статей. Класс
ArticleRepository представляет интерфейс для доступа к данным
о статьях. Различные реализации этого интерфейса могут хранить
данные в разных источниках, таких как файлы или базы данных.

В данном коде две конкретные реализации ArticleRepository
представлены: InFileArticleRepository для хранения данных
в файле и DatabaseArticleRepository для хранения данных
в базе данных. Класс DatabaseArticleService представляет
конкретный сервис для работы со статьями, хранящимися в базе
данных, который наследует абстрактный класс ArticleService и
использует DatabaseArticleRepository для управления данными.

В функции main() создается экземпляр DatabaseArticleRepository
и DatabaseArticleService, которые затем используются для создания
нескольких статей и получения заголовков и содержимого всех статей.

===========================================================

В данном коде принцип dependency inversion проявляется в том,
что зависимость между ArticleService и ArticleRepository
не является прямой. Вместо того, чтобы ArticleService
напрямую использовать конкретную реализацию ArticleRepository,
ArticleService зависит от абстрактного класса ArticleRepository,
который определяет только необходимые методы для доступа
к данным о статьях. Конкретные реализации ArticleRepository
наследуются от абстрактного класса и предоставляют специфические
реализации этих методов для работы с различными источниками данных.

Это позволяет легко заменять одну реализацию ArticleRepository
на другую без необходимости изменения кода ArticleService.
Например, если в будущем потребуется хранить данные о статьях
в другом источнике, достаточно будет создать новую конкретную
реализацию ArticleRepository и использовать ее вместо
существующей, не затрагивая ArticleService. Это соответствует
принципу dependency inversion, где более стабильные компоненты
зависят от менее стабильных абстракций.

=============================================================

В данном коде принцип dependency injection (DI) проявляется
в момент создания экземпляра класса DatabaseArticleService
в функции main(). Вместо того чтобы создавать объекты
DatabaseArticleService и DatabaseArticleRepository внутри класса
DatabaseArticleService или в других местах кода, они создаются
в функции main() и передаются в качестве аргументов конструктора
класса DatabaseArticleService. Это позволяет классу
DatabaseArticleService использовать различные реализации
ArticleRepository без изменения своего кода, так как реализация
ArticleRepository передается ему в виде аргумента. Таким образом,
зависимость DatabaseArticleService от DatabaseArticleRepository
инвертирована: вместо того чтобы создавать свою зависимость,
класс DatabaseArticleService получает ее извне. Это делает
код более гибким, позволяет удобно заменять реализации
зависимостей и упрощает тестирование.
*/

// BAD EXAMPLE: https://gist.github.com/sunmeat/0a799f98b3bba0aa5eb99bff681c142b
// GOOD EXAMPLE: https://gist.github.com/sunmeat/6d1223ca4efca9719a875c89e65b2103