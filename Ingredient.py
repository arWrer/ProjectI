#식약처 XML to CSV 파싱 프로그

# -*- coding: utf-8 -*-

import os
from bs4 import BeautifulSoup
from urllib.request import urlopen
import pandas as pd
import urllib.request

output_path = "C:\\Users\\USER\\Desktop\\창업프로젝트1"
output_file = "Ingredient.csv"

if not os.path.exists(output_path):
    os.makedirs(output_path)



df = pd.DataFrame(columns=['PRDCT_NM','PRIMARY_FNCLTY'])  

i=0

url = 'http://openapi.foodsafetykorea.go.kr/api/03043edf44444172a9ab/I2710/xml/1/1000'

data = urlopen(url).read()


soup = BeautifulSoup(data, "html.parser")


for item in soup.find_all('row'):

    df.loc[i] = [item.prdct_nm.text,item.primary_fnclty.text]


    i=i+1

df.to_csv(os.path.join(output_path, output_file), encoding='utf-8-sig', index=False)


