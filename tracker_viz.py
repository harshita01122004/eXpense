import sys
import os
try:
    import pandas as pd
    import matplotlib.pyplot as plt
except ImportError:
    print("Error: pandas and matplotlib are required.")
    print("Please run: pip install pandas matplotlib")
    sys.exit(1)

def main():
    if len(sys.argv) < 2:
        print("Usage: python tracker_viz.py <filename>")
        sys.exit(1)

    filename = sys.argv[1]

    if not os.path.exists(filename):
        print(f"Error: File '{filename}' not found.")
        print("Please add some expenses first in the C++ app.")
        sys.exit(1)

    try:
        # Read the CSV
        df = pd.read_csv(filename)
        
        # Check if empty or format is wrong
        if df.empty or 'Category' not in df.columns or 'Amount' not in df.columns:
            print("Error: The expense file is empty or formatted incorrectly.")
            sys.exit(1)
            
        # Group by category
        df['Amount'] = pd.to_numeric(df['Amount'], errors='coerce')
        category_sum = df.groupby('Category')['Amount'].sum()
        
        if category_sum.empty or category_sum.sum() == 0:
             print("No valid expense amounts to visualize.")
             sys.exit(1)

        # Plotting
        plt.figure(figsize=(10, 6))
        category_sum.plot(kind='pie', autopct='%1.1f%%', startangle=140, cmap='Set3')
        plt.title('Expense Breakdown by Category')
        plt.ylabel('')  # Hide the y-label
        
        print("Visualizing data... close the figure window to return to the application.")
        plt.show()

    except Exception as e:
        print(f"An error occurred while plotting: {e}")

if __name__ == "__main__":
    main()
