import random as r
import requests as rs
import time as t

while True:
  light = r.randint(0,7)
  temp = r.randint(0,4)
  url = "http://localhost:8000/esp/both/" + str(temp) + "&" + str(light)

  response = rs.post(url)
  if(response.status_code == 200):
    print("ok: \nluce: " + str(light) + "\ntemp: " + str(temp))
  else:
    print("Error: " + str(response))

  t.sleep(3)


