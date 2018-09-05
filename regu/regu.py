import re

string = '{/Line/1/Temperature={sourceTime=Wed Sep 05 15:30:15 CST 2018, ' \
         'sourcePicoSeconds=0, serverTime=Wed Sep 05 15:30:15 CST 2018, ' \
         'serverPicoSeconds=0, statusCode=0, statusCodeStr=Good, value=25}}'

strObj = re.match(r'\{(.*?)={sourceTime=(.*?), .*?=(.*?), .*?=(.*?), .*?=(.*?), .*?=(.*?), .*?=(.*?), .*value=(.*?)\}.*\}$', string)

print strObj.group(1)
print strObj.group(2)
print strObj.group(3)
print strObj.group(4)
print strObj.group(5)
print strObj.group(6)
print strObj.group(7)
print strObj.group(8)
