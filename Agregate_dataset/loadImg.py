from apiclient.discovery import build

service = build("customsearch", "v1",
                developerKey="AIzaSyB86RWORuTeqFAvQKe40iFmKVeUhCk4EIQ")

file = open('links.txt', 'a')
startNum = 0

for temp in range(1, 500):
    res = service.cse().list(
        q='advertising poster',
        cx='006921551596524514680:jjagjkuf4d8',
        searchType='image',
        num=10,
        safe='off',
        start=(temp * 10)
    ).execute()
    if not 'items' in res:
        print('No result !!\nres is: {}'.format(res))
    else:
        for item in res['items']:
            file.write(item['link'] + " \n")
       # print('{}:\n\t{}'.format(item['title'], item['link']))


# if not 'items' in res:
#     print('No result !!\nres is: {}'.format(res))
# else:
#     for item in res['items']:
#         file.write(item['link'] + " \n")
#         print('{}:\n\t{}'.format(item['title'], item['link']))
file.close()
