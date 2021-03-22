#!/usr/bin/env python3

from requests import get
from lxml import html

url = 'https://'
price_xpath = '//*[@id="product-price"]/text()'

# headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'}
headers = {'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:83.0) Gecko/20100101 Firefox/83.0'}

response = get(url, headers=headers)
tree = html.fromstring(response.content)
price = tree.xpath(price_xpath)

print(price)
