#!/usr/bin/env python3

import json, subprocess, os
from datetime import date
from requests import get
from lxml import html

# Site: 
site_name = "Generic"

# This changes for each site you are tracking:
price_xpath = '//div[1]/div[3]/div[1]/div[1]/p[3]/span[2]/text()'

def update_item(item, current_date):
	url = item["link"]
	headers = {'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_11_5) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/50.0.2661.102 Safari/537.36'}
	# headers = {'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:83.0) Gecko/20100101 Firefox/83.0'}
	try:
		response = get(url, headers=headers)
		tree = html.fromstring(response.content)
		price = tree.xpath(price_xpath)
		if not price:
			price = item["base_price"]
		else:
			price = price[0].split(" ")[0].replace(',', '.')
			price = float(price)
		item["prev_price"], item["current_price"] = item["current_price"], price
		item["date"] = current_date
		if item["current_price"] == item["prev_price"]:
			item["track"] = False
		else:
			item["track"] = True
		if item["base_price"] - item["current_price"] >= item["base_price"] * item["discount_alert"] / 100:
			item["alert"] = True
		else:
			item["alert"] = False
		if item["min_price"] > price:
			item["min_price"] = price
		if item["max_price"] < price:
			item["max_price"] = price
		if item["alert"] is True and item["prev_price"] != item["current_price"]:
			item["send_emails"] = True
		else:
			item["send_emails"] = False
	except:
		item["track"] = False
		item["alert"] = False
		item["send_emails"] = False


# Get today's date
current_date = str(date.today())

# Get script dir, json file, log file, notifications file
script_dir = os.path.dirname(os.path.realpath(__file__))
parent_dir = os.path.abspath(os.path.join(script_dir, os.pardir))
json_path = os.path.join(script_dir, 'products.json')
log_path = os.path.join(script_dir, 'prices.txt')
notifications_file = os.path.join(parent_dir, 'notifications.html')

# Load list of things to track
with open(json_path, 'r') as json_file:
	data = json.load(json_file)

# Update items
for item in data:
	update_item(item, current_date)

# Save new items to .json file
with open(json_path, 'w') as json_file:
	json.dump(data, json_file, indent=4)

# Save new prices to file
track = False
for item in data:
	if item["track"] is True:
		track = True
		break

if track is True:		
	with open(log_path, 'a+') as output_file:
		output_file.write("%-16s"% (current_date))
		for item in data:
			output_file.write("%-14.2f"% (item["current_price"]))
		output_file.write("\n")

# Send notification with notify-send and write to ../notifications.html
for item in data:
	if item["alert"] is True and item["foreign"] is False:
		output_string = f'{item["name"]}: {item["current_price"]}' \
						f' / {item["base_price"]}\n{item["link"]}\n' 
		subprocess.run(["notify-send", f'{site_name}:', output_string])

		output_string = f'<b>{site_name}:</b><br>\n{item["name"]}: {item["current_price"]}' \
						f' / {item["base_price"]}\n<br><a href=\"{item["link"]}\">{item["link"]}</a><br><br>\n\n' 
		with open(notifications_file, 'a+') as output_file:
			output_file.write(output_string)

# Send notification emails to mailing list
for item in data:
	for email_address in item["mailing_list"]:
		if item["send_emails"] is True: 
			output_string = f'{item["name"]}: {item["current_price"]}' \
							f' / {item["base_price"]}\n{item["link"]}\n' 
			subprocess.run(["bash", "-c", f'echo "{output_string}" | mail -s {item["name"]} {email_address}'])
