import requests, sys, threading, os, subprocess

# CONFIG
giteeGetLatestRelaeseApiUrl = (
    "https://gitee.com/api/v5/repos/bili9Blood/class-system-binaries/releases/latest"
)


def download_file(url, file_name):
    size = 0
    r = requests.get(url, stream=True)
    total = int(r.headers.get("content-length", 0))
    with open("tmp/" + file_name, "wb") as f:
        for chunk in r.iter_content(chunk_size=200000):
            if chunk:
                f.write(chunk)
                size += len(chunk)
                print("%.2f%%" % ((size / total) * 100))
    print("File complete download.")


def download_files(urls, file_names):
    os.system("mkdir tmp")
    threads = []
    print("{0} files.".format(len(urls)))
    for i in range(0, len(urls)):
        thread = threading.Thread(target=download_file, args=(urls[i], file_names[i]))
        threads.append(thread)
        thread.start()

    for thread in threads:
        thread.join()


def main():
    currentVer = sys.argv[1]
    remoteVer = dict(requests.get(giteeGetLatestRelaeseApiUrl).json())["tag_name"]
    if currentVer >= remoteVer:
        print("Everything up-to-date.")
        exit(0)

    remoteUrl = "https://gitee.com/bili9Blood/class-system-binaries/releases/download/{0}/".format(
        remoteVer
    )
    remoteUpdates = (
        requests.get(remoteUrl + "updates.txt").text.replace("\r", "").split("\n")
    )
    filesDownloadUrls = [""] * len(remoteUpdates)
    for i in range(0, len(remoteUpdates)):
        filesDownloadUrls[i] = remoteUrl + remoteUpdates[i]
    download_files(filesDownloadUrls, remoteUpdates)
    os.system(
        'powershell if($null -ne (Get-Process -Name "ClassSystem" -ErrorAction SilentlyContinue)){Stop-Process -Name "ClassSystem"}'
    )  # stop process to prepare copying files
    os.system(
        'powershell Copy-Item -Path "tmp/*" -Destination "./ -Recurse -force'
    )  # copy files

    subprocess.Popen(
        executable="ClassSystem.exe",
        args=[""],
    )
    os.system("powershell Remove-Item tmp -Recurse")


if __name__ == "__main__":
    if len(sys.argv) == 1:
        print("no arg")
        os._exit(1)
    main()
