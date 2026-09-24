# comment out
def quine():
    data = "# comment out\ndef quine():\n    data = %r\n    print(data %% data)\n\ndef main():\n    # comment in\n    quine()\n\nif __name__ == '__main__' :\n    main()"
    print(data % data)

def main():
    # comment in
    quine()

if __name__ == '__main__' :
    main()
