def is_ab_ending(string):
    states = {
        'start': 0,
        'a': 1,
        'ab': 2,
        'invalid': 3
    }
    
    current_state = states['start']
    
    for char in string:
        if current_state == states['invalid']:
            break
            
        if char == 'a':
            if current_state == states['start']:
                current_state = states['a']
            elif current_state == states['a']:
                current_state = states['a']
            elif current_state == states['ab']:
                current_state = states['a']
        elif char == 'b':
            if current_state == states['start']:
                current_state = states['invalid']
            elif current_state == states['a']:
                current_state = states['ab']
            elif current_state == states['ab']:
                current_state = states['invalid']
        else:
            current_state = states['invalid']
    
    return current_state == states['ab']

def main():   
    while True:
        user_input = input("\nEnter a string: ")
        
        result = is_ab_ending(user_input)
        
        if result:
            print(f"'{user_input}' ENDS with 'ab'")
        else:
            print(f"'{user_input}' doesn't end with 'ab' or it contains another symbols!")

if __name__ == "__main__":
    main()