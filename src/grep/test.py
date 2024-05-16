import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
import subprocess
import os
import pyperclip
import difflib

total_tests = 0

def run_tests():
    global total_tests
    options = ['-i', '-v', '-c', '-l', '-h', '-n', '-s', '-o']
    files = ['Makefile', 's21_grep.h', 's21_grep.c']

    total_tests = len(options)**3 + (len(options)-1) * len('iclnhsov')

    test_info_text.delete('1.0', tk.END)  # Clear previous results
    success_count = 0
    fail_count = 0
    test_number = 1

    for i, var1 in enumerate(options, start=1):
        for j, var2 in enumerate(options, start=1):
            for k, var3 in enumerate(options, start=1):
                test0 = f"-e while {var1} {var2} {var3} {' '.join(files)} -e handler"
                s21_grep_output = subprocess.run(['./s21_grep'] + test0.split(), stdout=subprocess.PIPE).stdout.decode('utf-8')
                grep_output = subprocess.run(['grep'] + test0.split(), stdout=subprocess.PIPE).stdout.decode('utf-8')

                if s21_grep_output == grep_output:
                    test_info_text.insert(tk.END, f"Test {test_number}: {test0} - SUCCESS\n")
                    success_count += 1
                else:
                    test_info_text.insert(tk.END, f"Test {test_number}: {test0} - FAIL\n")
                    fail_count += 1

                test_info_text.see(tk.END)  # Scroll to the end of the text field
                test_info_text.update()     # Update display
                test_number += 1
                update_progress(test_number, total_tests)

    result_count_label.config(text=f"Success: {success_count}, Fail: {fail_count}")

def update_progress(current, total):
    progress = min(int(current / total * 100), 100)
    progress_var.set(progress)

def show_test_info():
    def _show_test_info():
        test_number = int(test_number_entry.get())
        if 1 <= test_number <= total_tests:
            test_info_text.delete('1.0', tk.END)  # Clear the text field

            test_params, s21_output, grep_output = run_individual_test(test_number)

            test_info_text.insert(tk.END, f"Подробная информация для теста {test_number}:\n")
            test_info_text.insert(tk.END, f"Командные параметры: {test_params}\n\n")
            test_info_text.insert(tk.END, f"Вывод ./s21_grep:\n{s21_output}\n\n")
            test_info_text.insert(tk.END, f"Вывод grep:\n{grep_output}\n\n")

            diff = list(difflib.unified_diff(
                s21_output.splitlines(keepends=True),
                grep_output.splitlines(keepends=True),
                fromfile='s21_grep',
                tofile='grep'
            ))
            if diff:
                test_info_text.insert(tk.END, "Различия:\n" + ''.join(diff))
            else:
                test_info_text.insert(tk.END, "Нет различий в выводе.\n")
        else:
            messagebox.showwarning("Предупреждение", "Введите корректный номер теста.")

    test_info_window = tk.Toplevel(root)
    test_info_window.title("Введите номер теста")

    test_number_label = ttk.Label(test_info_window, text="Введите номер теста:")
    test_number_label.grid(column=0, row=0, sticky=tk.W)

    test_number_entry = ttk.Entry(test_info_window)
    test_number_entry.grid(column=1, row=0, sticky=tk.W)

    show_info_button = ttk.Button(test_info_window, text="Показать информацию о тесте", command=_show_test_info)
    show_info_button.grid(column=0, row=1, columnspan=2, sticky=tk.EW)

def run_individual_test(test_number):
    # Получение параметров теста на основе номера теста
    test_params = get_test_params(test_number)

    # Запуск команды ./s21_grep с тестовыми параметрами и получение вывода
    s21_grep_output = subprocess.run(['./s21_grep'] + test_params.split(), stdout=subprocess.PIPE).stdout.decode('utf-8')

    # Запуск стандартной команды grep с теми же параметрами и получение вывода
    grep_output = subprocess.run(['grep'] + test_params.split(), stdout=subprocess.PIPE).stdout.decode('utf-8')

    # Возврат параметров теста и результатов обеих команд
    return test_params, s21_grep_output, grep_output


def get_test_params(test_number):
    options = ['-i', '-v', '-c', '-l', '-h', '-n', '-s', '-o']
    files = ['Makefile', 's21_grep.h', 's21_grep.c']

    if test_number <= len(options)**3:
        i = (test_number - 1) // (len(options)**2)
        j = (test_number - 1) % (len(options)**2) // len(options)
        k = (test_number - 1) % (len(options)**2) % len(options)
        test_params = f"-e while {options[i]} {options[j]} {options[k]} {' '.join(files)} -e handler"
    else:
        test_number -= len(options)**3
        i = (test_number - 1) // len('iclnhsov')
        j = (test_number - 1) % len('iclnhsov') // len(options[:-1])
        k = (test_number - 1) % len(options[:-1]) % len(options[:-1])
        test_params = f"{options[i]}{options[:-1][j]} handler {' '.join(files[::-1])}"

    return test_params

def copy_to_clipboard():
    selected_text = test_info_text.get("1.0", "end-1c")
    pyperclip.copy(selected_text)

def about_program():
    messagebox.showinfo("About", "Made by haliseli@student.21-school.ru")

def run_tests_gui():
    if os.path.exists('./s21_grep'):
        run_tests()
    else:
        messagebox.showerror("Error", "s21_grep executable not found!")

def exit_app():
    root.destroy()

root = tk.Tk()
root.title("Тестирование s21_grep")
root.geometry("800x400")

style = ttk.Style(root)

frame = ttk.Frame(root, padding="20")
frame.grid(column=0, row=0, sticky=(tk.W, tk.E, tk.N, tk.S))
frame.columnconfigure(0, weight=1)
frame.rowconfigure(1, weight=1)

run_button = ttk.Button(frame, text="Run Tests", command=run_tests_gui)
run_button.grid(column=0, row=0, sticky=(tk.W, tk.E))

copy_button = ttk.Button(frame, text="Copy Text", command=copy_to_clipboard)
copy_button.grid(column=1, row=0, sticky=(tk.W, tk.E))

show_info_button = ttk.Button(frame, text="Show Test Info", command=show_test_info)
show_info_button.grid(column=2, row=0, sticky=(tk.W, tk.E))

about_button = ttk.Button(frame, text="About", command=about_program)
about_button.grid(column=3, row=0, sticky=(tk.W, tk.E))

test_info_text = tk.Text(frame, wrap='word', height=20, width=100)
test_info_text.grid(column=0, row=1, columnspan=4, sticky=(tk.N, tk.E, tk.S, tk.W))

result_count_label = ttk.Label(frame, text="")
result_count_label.grid(column=0, row=2, columnspan=4, sticky=(tk.W, tk.E))

progress_var = tk.IntVar()
progress_bar = ttk.Progressbar(frame, orient="horizontal", mode="determinate", variable=progress_var)
progress_bar.grid(column=0, row=3, columnspan=4, sticky=(tk.W, tk.E))

exit_button = ttk.Button(frame, text="Exit", command=exit_app)
exit_button.grid(column=0, row=4, columnspan=4, sticky=(tk.W, tk.E))

root.mainloop()

