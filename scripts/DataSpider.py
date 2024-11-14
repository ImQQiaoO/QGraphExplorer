from time import sleep
from tqdm import tqdm
import requests
import json
import re

JSON_ARR = []


def get_one_json_obj(i) -> bool:
    try:
        url = 'https://frodo.douban.com/api/v2/movie/' + i + '?apiKey=054022eaeae0b00e0fc068c0c0a2102a'
        headers = {
            'User-Agent': 'MicroMessenger/',
            'Referer': 'https://servicewechat.com/wx2f9b06c1de1ccfca/91/page-frame.html'
        }
        response = requests.get(url, headers=headers).json()
    except Exception as e:
        print(e)
        return False
    if response.get('msg') == 'subject_ip_rate_limit':
        print('error')
        return False
    JSON_ARR.append(response)
    return True


def get_top250():
    top_250 = []
    for i in range(0, 250, 25):
        url = 'https://movie.douban.com/top250?start=' + str(i) + '&filter='
        headers = {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/116.0.0.0 Safari/537.36'
        }
        response = requests.get(url, headers=headers)
        pattern = r'<a href="https://movie.douban.com/subject/(\d+)/" class="">'
        matches = re.findall(pattern, response.text)
        top_250.extend(matches)
        print(matches)
        sleep(3)
    print(top_250)
    with open('top_250.txt', 'w') as f:
        f.write('\n'.join(top_250))


def read_top250() -> list:
    with open('top_250.txt', 'r') as f:
        top_250 = [line.strip() for line in f.readlines()]
    return top_250


def get_top250_json():
    top_250 = read_top250()
    # continue_flag = True
    for i in tqdm(top_250):
        # if i == '1309163':    # 如果数据出现问题，可以从这里开始
        #     continue_flag = False
        # if continue_flag:
        #     continue
        condition = get_one_json_obj(i)
        if not condition:
            break
        sleep(5)
    with open('movie0.json', 'w', encoding='utf-8') as f:
        json.dump(JSON_ARR, f, ensure_ascii=False)  # type: ignore


def check_result() -> bool:
    top250 = read_top250()
    with open('movie.json', 'r', encoding='utf-8') as f:
        data = json.load(f)
    results = []
    for i in range(len(top250)):
        top_str = top250[i]
        data_str = json.dumps(data[i], ensure_ascii=False)
        if top_str in data_str:
            results.append((i, True))  # 如果匹配，返回 True
        else:
            results.append((i, False))  # 如果不匹配，返回 False
    has_error = False
    for idx, res in results:
        if not res:  # 只打印匹配为 False 的情况
            has_error = True
            print(f"Data[{idx}] does not contain top250[{idx}].")
    if not has_error:
        print("All data is correct.")
    return has_error


def main():
    # get_top250()
    # get_top250_json()
    check_result()


if __name__ == '__main__':
    main()
