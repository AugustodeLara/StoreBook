package com.example.bookStore.ui.main.adapter;

import android.content.Context;
import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;

import com.example.bookStore.ui.main.BookDetailActivity;
import com.example.curlssl.R;
import com.squareup.picasso.Picasso;

import java.util.List;

public class BookAdapter extends RecyclerView.Adapter<BookAdapter.ViewHolder> {

    private Context context;
    private List<String> books;

    public BookAdapter(List<String> books, Context context) {
        this.books = books;
        this.context = context;
    }

    public void addBook(String id, String title, String description, String thumbnailUrl) {
        books.add(id);
        books.add(title);
        books.add(description);
        books.add(thumbnailUrl);
        notifyItemInserted(books.size() / 4);
    }

    public static class ViewHolder extends RecyclerView.ViewHolder {
        public TextView textTitle;
        public TextView textDescription;
        public ImageView imageThumbnail;

        public ViewHolder(@NonNull View itemView) {
            super(itemView);
            textTitle = itemView.findViewById(R.id.textTitle);
            textDescription = itemView.findViewById(R.id.textDescription);
            imageThumbnail = itemView.findViewById(R.id.imageThumbnail);
        }
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext())
                .inflate(R.layout.item_book, parent, false);
        return new ViewHolder(view);
    }

    public String removeDoubleAsterisks(String input) {
        return input.replace("**", "");
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        int dataIndex = position * 6;

        if (dataIndex + 5 < books.size()) {
            String id = books.get(dataIndex);
            String title = books.get(dataIndex + 1);
            String description = books.get(dataIndex + 2);
            String thumbnailUrl = books.get(dataIndex + 3);
            String favorite = books.get(dataIndex + 4);
            String buyLink = books.get(dataIndex + 5);

            holder.textTitle.setText(removeDoubleAsterisks(title));
            holder.textDescription.setText(removeDoubleAsterisks(description));
            Picasso.get().load(removeDoubleAsterisks(thumbnailUrl)).into(holder.imageThumbnail);

            holder.itemView.setOnClickListener(view -> openBookDetails(id, title, description, thumbnailUrl, favorite, buyLink));
        }
    }

    private void openBookDetails(String id, String title, String description, String thumbnailUrl, String favorite, String buyLink) {
        Intent intent = new Intent(context, BookDetailActivity.class);
        intent.putExtra("id", removeDoubleAsterisks(id));
        intent.putExtra("title", removeDoubleAsterisks(title));
        intent.putExtra("description", removeDoubleAsterisks(description));
        intent.putExtra("thumbnailUrl", removeDoubleAsterisks(thumbnailUrl));
        intent.putExtra("favorite", Boolean.parseBoolean(removeDoubleAsterisks(favorite)));
        intent.putExtra("buyLink", removeDoubleAsterisks(buyLink));
        context.startActivity(intent);
    }

    @Override
    public int getItemCount() {
        return books.size() / 6;
    }
}
