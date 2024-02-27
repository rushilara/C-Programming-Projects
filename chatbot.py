import openai
import os

# Securely load your API key from an environment variable or a secure file
api_key = os.getenv("OPENAI_API_KEY")
client = openai.OpenAI(api_key=api_key)

# Initialize a list to keep track of the conversation history
conversation_history = []

def chat_with_gpt(prompt):
    # Add the user's prompt to the conversation history
    conversation_history.append({"role": "user", "content": prompt})
    
    try:
        response = client.chat.completions.create(
            model="gpt-3.5-turbo",
            messages=conversation_history
        )
        # Extract the model's response and add it to the conversation history
        model_response = response.choices[0].message['content'].strip()
        conversation_history.append({"role": "assistant", "content": model_response})
        
        return model_response
    except openai.RateLimitError:
        print("Rate limit exceeded. Please try again later.")
        return None
    except openai.OpenAIError as e:
        print(f"An OpenAI API error occurred: {e}")
        return None
    except Exception as e:  # Broad exception handling for unexpected issues
        print(f"An unexpected error occurred: {e}")
        return None

if __name__ == "__main__":
    while True:
        experience = input("What is your experience level? (Beginner, Intermediate, Expert)")
        if experience.lower() in ["beginner", "intermediate", "expert"]:
            break
        else:
            print("Must respond with Beginner, Intermediate, or Expert.\n")
    height = input("What is your height? (Make sure to specify your unit of measurement (ex: 6 feet))")
    weight = input("What is your weight? (Make sure to specify your unit of measurement (ex: 180 pounds))")
    gender = input("What is your gender? (Male, female, other..) ")
    goal = input("What is your fitness goal? (Ex: lose 10 pounds) ")
    user_input = "Act as a fitness trainer and generate a fitness plan for a " + height + " tall, " + weight + gender + " whose goal is to " + goal
    print(user_input)
    response = chat_with_gpt(user_input)
    if response:
        print("Fitness Chat:", response)
    else:
        print("Unable to generate a response at this time.")
    print("Welcome to the fitness chatbot Feel free to ask anymore questions!. Type 'quit', 'exit', or 'bye' to quit.")
    while True:
        # Collect user input
        user_input = input("You: ")
        
        # Check if the user wants to exit
        if user_input.lower() in ["quit", "exit", "bye"]:
            print("Goodbye!")
            break

        # Get the chatbot's response and print it
        response = chat_with_gpt(user_input)
        if response:
            print("Fitness Chat:", response)
        else:
            print("Unable to generate a response at this time.")

