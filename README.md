# The bank's application 

## The idea of the application 
The application was created on my own initiative, because I wanted to create a really necessary application. Nowadays, not everyone is interested in financial literacy, but my application will help with this. It allows you to track your income and expenses, calculate loan payments and follow the latest economic and currency exchange rate news.

## **Key features**
I had already created an application in QT before, so it was easier to create an application. But still, I faced some difficulties.
- **Authorization.** In this application, I have added password hashing for greater security. You can also go to the registration page and register if you are a new user. After successful authorization, a menu opens in which you can choose which section of the application you are more interested in.
- **Analysis of expenses and income.** The user can enter their data on expenses and income and they will be saved for further launches. For clarity, I use a graph and pie charts. The graph shows the user's balance on different days. There are two pie charts - one shows income by category, and the other shows expenses.
- **Currency market.** Here the user can view the current exchange rate. Information is parsed from the site using the curl library. The search by name is organized in this section. It is looking for the currency you need even with a half-word.
- **Currency converter.** It uses the same information that is parsed from the site in the previous section. Unfortunately, the converter is not as accurate, since it converts everything into rubles to convert to any other currency.
- **News.** The latest news for today is shown here. Clicking on the headline of the news will open the browser with the original article. It also uses parsing from the site using the curl library.
- **Loan calculator.** This section does not use third-party information, all calculations take place inside the program.

## Program structure
My program is divided into classes thematically. The main window class connects them only to each other and to the application itself, and also connects the database used throughout the application.
The authorization class is responsible for registering and authorizing users. The income_and_expenses class is responsible for the financial analysis section of the user. The add_action class is responsible for the window for adding expenses or income, and the del_action class is responsible for the window for deleting expenses or income. The currencies class is responsible for the currency exchange rate tracking section. The converter class is responsible for the currency conversion section. The news class is responsible for the news section from the world of economics. The loan class is responsible for the loan calculator section, and the add_rate class is responsible for the window for adding the loan interest rate and calculating payments.
