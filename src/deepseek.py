from openai import OpenAI
import os

# Configure Deepseek client
client = OpenAI(
    base_url="https://api.deepseek.com/",  # Verify endpoint with Deepseek's docs
    api_key=os.getenv("DEEPSEEK_API_KEY"),  # Set your API key in environment
)

def chat_with_deepseek():
    messages = [
        {"role": "system", "content": "You are a helpful assistant."},
    ]
    
    print("Chat with Deepseek Assistant (type 'quit', 'exit', or 'bye' to end)")
    
    while True:
        user_input = input("You: ")
        
        if user_input.lower().strip() in ['quit', 'exit', 'bye']:
            print("Assistant: Goodbye!")
            break
            
        messages.append({"role": "user", "content": user_input})
        
        try:
            response = client.chat.completions.create(
                model="deepseek-chat",  # Verify correct model name
                messages=messages,
                temperature=0.7,
                # Add any Deepseek-specific parameters here
            )
            
            assistant_response = response.choices[0].message.content
            print(f"Assistant: {assistant_response}")
            messages.append({"role": "assistant", "content": assistant_response})
            
        except Exception as e:
            print(f"Error: {e}")
            break

if __name__ == "__main__":
    chat_with_deepseek()