cmake_minimum_required(VERSION 3.5)
project(messenger)

add_subdirectory(client)
add_subdirectory(server)

# Создаем цель для сборки клиента
add_custom_target(Client DEPENDS messengerClientTarget)

# Создаем цель для сборки сервера
add_custom_target(Server DEPENDS messengerServerTarget)