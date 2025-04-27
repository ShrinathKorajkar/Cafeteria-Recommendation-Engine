import os
script_dir = os.path.dirname(os.path.abspath(__file__))

# File paths
negative_sentiment_file_path = os.path.join(script_dir,'../../assets/negativeSentiments.csv')
positive_sentiment_file_path = os.path.join(script_dir,'../../assets/positiveSentiments.csv')
comment_file_path = os.path.join(script_dir,'../../assets/comments.txt')
output_file_path = os.path.join(script_dir, '../../assets/commentsDb.txt')

def read_sentiments(file_path):
    with open(file_path, 'r') as file:
        sentiments = [line.strip().lower() for line in file]
    return sentiments

# Check if any sentiment is present in the comment
def print_sentiment(comment, pos_sentiments, neg_sentiments, current_no):
    comment_lower = comment.lower()
    with open(output_file_path, 'a') as file:
        for sentiment in pos_sentiments:
            if sentiment in comment_lower:
                line = "(" + str(current_no) + ", '" + sentiment + "', 'positive'),"
                file.write(line + '\n')
        for sentiment in neg_sentiments:
            if sentiment in comment_lower:
                line = "(" + str(current_no) + ", '" + sentiment + "', 'negative'),"
                file.write(line + '\n')

# Process comments and print those with matching sentiments
def process_comments(comment_file_path, pos_sentiments, neg_sentiments, start_no, end_no):
    with open(comment_file_path, 'r') as file:
        current_no = start_no
        for line in file:
            comment = line.strip()
            print_sentiment(comment, pos_sentiments, neg_sentiments, current_no)
            current_no += 1
            if current_no > end_no:
                current_no = start_no

# Main execution
start_no = 113
end_no = 158

# clear the contents of output file before writing
with open(output_file_path, 'w') as file:
    pass

neg_sentiments = read_sentiments(negative_sentiment_file_path)
pos_sentiments = read_sentiments(positive_sentiment_file_path)
process_comments(comment_file_path, pos_sentiments, neg_sentiments, start_no, end_no)
