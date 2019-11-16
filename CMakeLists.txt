cmake_minimum_required(VERSION 3.2) # ustawiamy minimalna wersje cmake

include("${CMAKE_CURRENT_LIST_DIR}/cmake/hunter.cmake") # inkludujemy huntera on musi byc przed deklaracja projektu

# ustawiamy standart c++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)


project(OpenGL-learn VERSION 1.0.0) # tworzymy projekt


# dodajemy biblioteki z ktorych bedziemy korzystac
hunter_add_package(stb)
find_package(stb CONFIG REQUIRED)

hunter_add_package(glfw)
find_package(glfw3 REQUIRED)

hunter_add_package(glew)
find_package(glew CONFIG REQUIRED)

hunter_add_package(glm)
find_package(glm CONFIG REQUIRED)


# przeszukujemy pliki zamiast je podawac rzecznie
set(SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/OpenGL-learn")
file(GLOB headers "${SOURCE_DIR}/*.h")
file(GLOB sources "${SOURCE_DIR}/*.cpp")


# tworzymy target czyli nasza aplikacje
add_executable(${PROJECT_NAME} ${headers} ${sources})                      # dodajemy target ktory bedzie kompilowany jako program oraz ustawiamy jego nazw na nazwe projektu
target_include_directories(${PROJECT_NAME} PUBLIC "${SOURCE_DIR}")         # ustamaly z jakiej lokalizacji projekt bedzie pobierac naglowki
target_link_libraries(${PROJECT_NAME} PUBLIC stb::stb glfw glew::glew glm) # Ustawiamy linkera tak zeby podlaczyl biblioteki ktore hunter dodal


# tworzymy kopie plikow resources tak aby program mogl z nich korzystac
# przeszukujemy pliki resources żeby moc je przkopiowac
set(RESOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/resources")
file(GLOB_RECURSE resources "${RESOURCE_DIR}/**")


# 
foreach(filepath IN LISTS resources)

	# Pobieramy nazwy plikow oraz folderow
	get_filename_component(filename ${filepath} NAME)
	get_filename_component(dir ${filepath} DIRECTORY)
	get_filename_component(basedir ${dir} NAME)

	# Sprawdzamy na jakiej platformie jestesmy
	if(WIN32)
		# nie jestem pewienn czy wersja na windowsa dziala

		# Ustawiamy zmienna zeby zmiejszyc dlugosc funkcji
		set(RESOURCE_OUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}")

		# Tworzymy wersje debug
		add_custom_command(
			OUTPUT "${RESOURCE_OUT_DIR}/Debug/resources/${basedir}/${filename}"
			DEPENDS "${filepath}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_OUT_DIR}/Debug/resources/${basedir}"
			COMMAND ${CMAKE_COMMAND} -E copy "${filepath}" "${RESOURCE_OUT_DIR}/Debug/resources/${basedir}/${filename}"
			COMMENT "${PROJECT_NAME} copy resource file: ${filename}"
		)

		# Tworzymy wersje release
		add_custom_command(
			OUTPUT "${RESOURCE_OUT_DIR}/Release/resources/${basedir}/${filename}"
			DEPENDS "${filepath}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${RESOURCE_OUT_DIR}/Release/resources/${basedir}"
			COMMAND ${CMAKE_COMMAND} -E copy "${filepath}" "${RESOURCE_OUT_DIR}/Release/resources/${basedir}/${filename}"
			COMMENT "${PROJECT_NAME} copy resource file: ${filename}"
		)

		# Dodajemy sciezke pliku do tabily, w ktorej znajduja sie wyjsciowe pliki
		list(APPEND resources_out "${RESOURCE_OUT_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>/resources/${basedir}/${filename}")

	else()

		# Tworzymy custom command ktora kopiuje pliki kiedy zmieni sie bazowy plik
		add_custom_command(
			OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/resources/${basedir}/${filename}"
			DEPENDS "${filepath}"
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/resources/${basedir}"
			COMMAND ${CMAKE_COMMAND} -E copy "${filepath}" "${CMAKE_CURRENT_BINARY_DIR}/resources/${basedir}/${filename}"
			COMMENT "${PROJECT_NAME} copy resource file: ${filename}"
		)

		# Dodajemy sciezke pliku do tabily, w ktorej znajduja sie wyjsciowe pliki
		list(APPEND resources_out "${CMAKE_CURRENT_BINARY_DIR}/resources/${basedir}/${filename}")

	endif()
	
endforeach()


# Tworzymy custom target ktory ma przekopiowac pliki resources 
add_custom_target(${PROJECT_NAME}_out ALL DEPENDS ${resources_out})
add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}_out)


# Ustawiamy grupy to daje przejrzysty projekt
source_group("Header Files" FILES ${headers})
source_group("Source Files" FILES ${sources})
source_group("Resources Files" FILES ${resources})
