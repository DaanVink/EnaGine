import argparse

parser = argparse.ArgumentParser()

parser.add_argument("-test", action = "store_true")

args = parser.parse_args()

print(args.test)