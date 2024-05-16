import os
import tkinter as tk
from tkinter import ttk, scrolledtext

TEST_FILE = "bytes.txt"
LOG_FILE = "log.txt"

def run_tests():
    counter_success = 0
    counter_fail = 0
    number = 1
    result_text.delete(1.0, tk.END)

    with open(LOG_FILE, "w") as log_file:
        for var in ['-b', '-e', '-n', '-s', '-t', '-v']:
            test1 = f"{var} {TEST_FILE}"
            os.system(f"./s21_cat {test1} > s21_cat.txt")
            os.system(f"cat {test1} > cat.txt")
            diff_res = os.system("diff -s s21_cat.txt cat.txt > /dev/null")
            if diff_res == 0:
                result_text.insert(tk.END, f"TEST {number} - SUCCESS\n", "success")
                counter_success += 1
            else:
                result_text.insert(tk.END, f"TEST {number} - FAIL\n", "fail")
                log_file.write(f"{test1}\n")
                counter_fail += 1
            number += 1
            os.remove("s21_cat.txt")
            os.remove("cat.txt")
            result_text.see(tk.END)
            result_text.update()

        for var in ['-b', '-e', '-n', '-s', '-t', '-v', '-e']:
            for var2 in ['-b', '-e', '-n', '-s', '-t', '-v', '-b']:
                if var != var2:
                    test1 = f"{var} {var2} {TEST_FILE}"
                    os.system(f"./s21_cat {test1} > s21_cat.txt")
                    os.system(f"cat {test1} > cat.txt")
                    diff_res = os.system("diff -s s21_cat.txt cat.txt > /dev/null")
                    if diff_res == 0:
                        result_text.insert(tk.END, f"TEST {number} - SUCCESS\n", "success")
                        counter_success += 1
                    else:
                        result_text.insert(tk.END, f"TEST {number} - FAIL\n", "fail")
                        log_file.write(f"{test1}\n")
                        counter_fail += 1
                    number += 1
                    os.remove("s21_cat.txt")
                    os.remove("cat.txt")
                    result_text.see(tk.END)
                    result_text.update()

        for var in ['-b', '-e', '-n', '-s', '-t', '-v']:
            for var2 in ['-b', '-e', '-n', '-s', '-t', '-v']:
                for var3 in ['-b', '-e', '-n', '-s', '-t', '-v']:
                    if var != var2 and var != var3 and var2 != var3:
                        test1 = f"{var} {var2} {var3} {TEST_FILE}"
                        os.system(f"./s21_cat {test1} > s21_cat.txt")
                        os.system(f"cat {test1} > cat.txt")
                        diff_res = os.system("diff -s s21_cat.txt cat.txt > /dev/null")
                        if diff_res == 0:
                            result_text.insert(tk.END, f"TEST {number} - SUCCESS\n", "success")
                            counter_success += 1
                        else:
                            result_text.insert(tk.END, f"TEST {number} - FAIL\n", "fail")
                            log_file.write(f"{test1}\n")
                            counter_fail += 1
                        number += 1
                        os.remove("s21_cat.txt")
                        os.remove("cat.txt")
                        result_text.see(tk.END)
                        result_text.update()

        for var in ['-b', '-e', '-n', '-s', '-t', '-v']:
            for var2 in ['-b', '-e', '-n', '-s', '-t', '-v']:
                for var3 in ['-b', '-e', '-n', '-s', '-t', '-v']:
                    for var4 in ['-b', '-e', '-n', '-s', '-t', '-v']:
                        if var != var2 and var != var3 and var != var4 and var2 != var3 and var2 != var4 and var3 != var4:
                            test1 = f"{var} {var2} {var3} {var4} {TEST_FILE}"
                            os.system(f"./s21_cat {test1} > s21_cat.txt")
                            os.system(f"cat {test1} > cat.txt")
                            diff_res = os.system("diff -s s21_cat.txt cat.txt > /dev/null")
                            if diff_res == 0:
                                result_text.insert(tk.END, f"TEST {number} - SUCCESS\n", "success")
                                counter_success += 1
                            else:
                                result_text.insert(tk.END, f"TEST {number} - FAIL\n", "fail")
                                log_file.write(f"{test1}\n")
                                counter_fail += 1
                            number += 1
                            os.remove("s21_cat.txt")
                            os.remove("cat.txt")
                            result_text.see(tk.END)
                            result_text.update()

        result_text.insert(tk.END, f"SUCCESS {counter_success}\n", "success")
        result_text.insert(tk.END, f"FAIL {counter_fail}\n", "fail")
        result_text.see(tk.END)
        result_text.update()

def clear_results():
    result_text.delete(1.0, tk.END)
    if os.path.exists(LOG_FILE):
        os.remove(LOG_FILE)

def quit_program():
    root.quit()
    root.destroy()

# GUI setup
root = tk.Tk()
root.title("Test Runner")

frame = ttk.Frame(root)
frame.grid(row=0, column=0, padx=10, pady=10)

run_button = ttk.Button(frame, text="Run Tests", command=run_tests)
run_button.grid(row=0, column=0, padx=5, pady=5)

clear_button = ttk.Button(frame, text="Clear Results", command=clear_results)
clear_button.grid(row=0, column=1, padx=5, pady=5)

quit_button = ttk.Button(frame, text="Quit", command=quit_program)
quit_button.grid(row=0, column=2, padx=5, pady=5)

result_text = scrolledtext.ScrolledText(frame, height=20, width=50, wrap=tk.WORD)
result_text.grid(row=1, column=0, columnspan=3, padx=5, pady=5)

# Define tag configurations for colored output
result_text.tag_config("success", foreground="green")
result_text.tag_config("fail", foreground="red")

root.mainloop()

