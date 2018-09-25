import json
from pprint import pprint

data = json.loads("{}")

data["test"] = "test"
data["test1"] = "test1"
data["test2"] = "test2"

pprint(data)
print(data)