cmake_minimum_required(VERSION 3.20)
project(Lab3_OOP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -Wextra)

add_executable(Lab3_OOP
        main.cpp
        credit_card.cpp
        client.cpp
        order.cpp
        payment.cpp
        administrator.cpp
        bank_account.cpp
        current_account.cpp
        savings_account.cpp
        credit_account.cpp
        installment_plan.cpp
        insurance.cpp
        loyalty_program.cpp
        premium_account.cpp
        exceptions.cpp
        financial_product.cpp
        legacy_transaction.cpp
        transfer_transaction.cpp
        product_store.cpp
        service.cpp
        lab5_demo.cpp)
