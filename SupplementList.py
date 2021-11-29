# 식약처 XML to CSV 파싱프로그램

# -*- coding: utf-8 -*-

import os
from bs4 import BeautifulSoup
from urllib.request import urlopen
import pandas as pd
import urllib.request

output_path = "C:\\Users\\USER\\Desktop\\startup1"
output_file = "SupplementList.csv"

if not os.path.exists(output_path):
    os.makedirs(output_path)



df = pd.DataFrame(columns=['PRDLST_NM', 'PRIMARY_FNCLTY', 'DISPOS', 'NTK_MTHD'])

i=0

url = 'http://openapi.foodsafetykorea.go.kr/api/03043edf44444172a9ab/C003/xml/1/1000'

data = urlopen(url).read()


soup = BeautifulSoup(data, "html.parser")


for item in soup.find_all('row'):

    df.loc[i] = [item.prdlst_nm.text, item.primary_fnclty.text, item.dispos.text, item.ntk_mthd.text,
        ]


    i=i+1

df.to_csv(os.path.join(output_path, output_file), encoding='utf-8-sig', index=False)


