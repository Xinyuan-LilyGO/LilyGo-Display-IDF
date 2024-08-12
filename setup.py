import os
import shutil

current_directory = os.getcwd()
sdkconfig_path = os.path.join(current_directory, "sdkconfig")
output_path = os.path.join(current_directory, "build")
idf_component_path = os.path.join(current_directory, "main")
idf_component_path = os.path.join(idf_component_path, "idf_component.yml")

os.system("git submodule init")
os.system("git submodule update")

if os.path.isfile(idf_component_path):
    os.remove(idf_component_path)
if os.path.isfile(sdkconfig_path):
    print("Delete temporary files")
    os.remove(sdkconfig_path)
if os.path.exists(output_path):
    print("Delete temporary files")
    shutil.rmtree(output_path)

r = os.system("idf.py > " + os.devnull)
if r != 0:
    print(
        "Unable to execute idf.py, please see here to learn how to use and install https://github.com/espressif/esp-idf"
    )
    exit()


def copy_file(source_path, destination_path):
    try:
        shutil.copy(source_path, destination_path)
        print(f"Set default compilation configuration")
    except FileNotFoundError:
        print("File not found, please check the file path.")
    except PermissionError:
        print("No permission to access file, please check file permissions.")
    except Exception as e:
        print(f"An error occurred: {e}")


def show_menu():
    print("1.  T4-S3")
    print("2.  T-AMOLED-S3 (Touch or NoTouch)")
    print("3.  T-AMOLED-LITE (16M Flash 8M PSRAM)")
    print("4.  T-QT-S3 FN4R2 (4M Flash 2M PSRAM)")
    print("5.  T-QT-S3 FN8 (8M Flash No PSRAM)")
    print("6.  T-Display")
    print("7.  T-Display-S3")
    print("8.  T-Display-S3-Pro")
    print("9.  T-Dongle-S2")
    print("10. T-Dongle-S3")
    print("11. T-HMI")
    print("12. T-Display-Long")
    print("13. T-QT-C6")
    print("14. T-RGB (2.1 or 2.8 Inches)(Support FT5236,CST820,GT911)")
    print("15. T-Watch S3")


def get_user_choice():
    while True:
        try:
            choice = int(
                input(
                    "Please enter the board number that needs to be compiled (1-15): "
                )
            )
            if 1 <= choice <= 15:
                return choice
            else:
                print("Invalid input, please enter a number between 1 and 15.")
        except ValueError:
            print("Invalid input, please enter a valid number.")


def perform_action(choice):
    if choice == 1:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t4-s3"),
            "sdkconfig.defaults",
        )
    elif choice == 2:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-amoled-s3"),
            "sdkconfig.defaults",
        )
    elif choice == 3:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-amoled-lite"),
            "sdkconfig.defaults",
        )
    elif choice == 4:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-qt-s3-fn4r2"),
            "sdkconfig.defaults",
        )
        os.system('idf.py add-dependency "espressif/esp_lcd_gc9a01^2.0.0"')
    elif choice == 5:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-qt-s3-fn8"),
            "sdkconfig.defaults",
        )
        os.system('idf.py add-dependency "espressif/esp_lcd_gc9a01^2.0.0"')
    elif choice == 6:
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-display"),
            "sdkconfig.defaults",
        )
        os.system("idf.py set-target esp32")
    elif choice == 7:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-display-s3"),
            "sdkconfig.defaults",
        )
    elif choice == 8:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-display-s3pro"),
            "sdkconfig.defaults",
        )
        os.system('idf.py add-dependency "espressif/esp_lcd_st7796^1.2.1"')
    elif choice == 9:
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-dongle-s2"),
            "sdkconfig.defaults",
        )
        os.system("idf.py set-target esp32s2")
    elif choice == 10:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-dongle-s3"),
            "sdkconfig.defaults",
        )
    elif choice == 11:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-hmi"),
            "sdkconfig.defaults",
        )
        os.system('idf.py add-dependency "atanisoft/esp_lcd_touch_xpt2046^1.0.2"')
    elif choice == 12:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-display-long"),
            "sdkconfig.defaults",
        )
    elif choice == 13:
        os.system("idf.py set-target esp32c6")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-qt-c6"),
            "sdkconfig.defaults",
        )
        os.system('idf.py add-dependency "espressif/esp_lcd_gc9a01^2.0.0"')
    elif choice == 14:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-rgb"),
            "sdkconfig.defaults",
        )

    elif choice == 15:
        os.system("idf.py set-target esp32s3")
        copy_file(
            os.path.join(current_directory, "sdkconfig.defaults.t-watch-s3"),
            "sdkconfig.defaults",
        )

    os.system('idf.py add-dependency "lvgl/lvgl^8.3.11"')

    os.system("idf.py reconfigure")

    os.system("idf.py build")


if __name__ == "__main__":
    show_menu()
    user_choice = get_user_choice()
    perform_action(user_choice)
