import random
import matplotlib.pyplot as plt

def get_data(filename,cooling_tube_number):
    import numpy as np
    import csv

    with open(filename, newline='') as csvfile:
        reader = csv.reader(csvfile, delimiter=',', quotechar='|')
        time = {}
        temp = {}
        i=0
        ind=1+cooling_tube_number*3
        for row in reader:
                time[i] = row[0]
                temp[i] = row[ind]
                i=i+1
    
    time_list = list(time.values()) 
    time_list=time_list[1:4330]
    #print(time_list)
    time_numbers = [ float(x) for x in time_list]
    #print(time_numbers)

    temp_list = list(temp.values())  
    notes=temp_list[0:1]
    print(notes)
    temp_list=temp_list[1:4330]
    #print(temp_list)
    temp_numbers = [ float(x) for x in temp_list]
    #print(force_numbers)
    
    return [time_numbers, temp_numbers, notes]


#just input the module number and get it's corresponding temp graph
def module(num):
    for i in range(7):
        r = random.random()
        b = random.random()
        g = random.random()
        color = (r, g, b) #these are just to get distint colors, if color not distinct, run again

        [t, T, l] = get_data('June_10_Beta30_Thermals.csv', i+7*(num-1))
        m1 = plt.figure(num)
        plt.plot(t, T, label=l, c=color)

    plt.legend()
    plt.grid(True)
    plt.title("Module %i Temperature vs. Time Pack Heating/Cooling" %num)
    plt.xlabel("Time (s)")
    plt.ylabel("Temperature (C)")
    plt.show()


#plots the first tube from each module
def firstInModule():

    for i in range(12):
        r = random.random()
        b = random.random()
        g = random.random()
        color = (r, g, b)

        [t, T, l] = get_data('June_10_Beta30_Thermals.csv', 7 * i)
        m1 = plt.figure(1)
        plt.plot(t, T, label=l, c=color)

    plt.legend()
    plt.grid(True)
    plt.title("Module's First Tube Temperature vs. Time Pack Heating/Cooling")
    plt.xlabel("Time (s)")
    plt.ylabel("Temperature (C)")
    plt.show()


"""
module(1)
module(2)
module(3)
module(4)
module(5)
module(6)
module(7)
module(8)
module(9)
module(10)
module(11)
module(12)
"""
firstInModule()
