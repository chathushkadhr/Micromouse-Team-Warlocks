import tkinter as tk

def button_click(text):
    label.config(text=text)

# Create the main application window
root = tk.Tk()
root.title("Dynamic Buttons")

# Create a label to display the button click result
label = tk.Label(root, text="", font=("Helvetica", 24))
label.pack(pady=20)

# Button texts to create
button_texts = ["Button 1", "Button 2", "Button 3"]

# Create buttons using a loop
buttons = []
for text in button_texts:
    button = tk.Button(root, text=text, command=lambda t=text: button_click(t), bg="black", fg="white", width=10, height=2)
    buttons.append(button)

# Place the buttons vertically
for i, button in enumerate(buttons):
    button.place(x=0, y=i * 70, width=100, height=50)

# Start the main event loop
root.mainloop()
